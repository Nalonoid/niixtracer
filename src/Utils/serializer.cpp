#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QFile>
#include <QTextStream>

#include "serializer.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Image/image.hpp"
#include "Object/Camera/camera.hpp"
#include "Object/Light/light.hpp"
#include "Scene/scene.hpp"
#include "Material/fluorescence.hpp"

Serializer::Serializer() : _scene(nullptr) {}

Serializer::~Serializer()
{
    delete _scene;
}

void Serializer::init(Scene *s)
{
    _scene = s;
}

void Serializer::write_to_XML(const std::string &output_path)
{
    QDomDocument scene_doc;
    const Image *img = _scene->_output_img;

    QDomElement scene_elem = scene_doc.createElement("scene");
    scene_doc.appendChild(scene_elem);

    QDomElement settings_elem = scene_doc.createElement("settings");
    scene_elem.appendChild(settings_elem);

    QDomElement path_elem   = scene_doc.createElement("path");
    QDomElement xres_elem   = scene_doc.createElement("xres");
    QDomElement yres_elem   = scene_doc.createElement("yres");
    QDomElement mode_elem   = scene_doc.createElement("mode");
    QDomElement depth_elem  = scene_doc.createElement("depth");
    QDomElement sample_elem = scene_doc.createElement("sampl");

    QDomText path   = scene_doc.createTextNode(QString::fromStdString(_scene->output_path()));
    QDomText xres   = scene_doc.createTextNode(QString::number(img->width()));
    QDomText yres   = scene_doc.createTextNode(QString::number(img->height()));
    QDomText mode   = scene_doc.createTextNode(QString::fromStdString(_scene->_mode));
    QDomText depth  = scene_doc.createTextNode(QString::number(_scene->max_depth()));
    QDomText sample = scene_doc.createTextNode(QString::number(_scene->nb_samples()));

    path_elem.appendChild(path);
    xres_elem.appendChild(xres);
    yres_elem.appendChild(yres);
    mode_elem.appendChild(mode);
    depth_elem.appendChild(depth);
    sample_elem.appendChild(sample);

    settings_elem.appendChild(path_elem);
    settings_elem.appendChild(xres_elem);
    settings_elem.appendChild(yres_elem);
    settings_elem.appendChild(mode_elem);
    settings_elem.appendChild(depth_elem);
    settings_elem.appendChild(sample_elem);

    for (auto itr = _scene->cameras().begin(); itr < _scene->cameras().end(); itr++)
    {
        QDomElement camera_elem = scene_doc.createElement("camera");
        scene_elem.appendChild(camera_elem);

        QDomElement position_elem = scene_doc.createElement("position");
        scene_elem.appendChild(position_elem);

        QDomElement look_at_elem = scene_doc.createElement("look_at");
        scene_elem.appendChild(look_at_elem);

        QDomText pos = scene_doc.createTextNode(
                    QString::fromStdString((*itr)->position().to_string()));

        QDomText lkat = scene_doc.createTextNode(
                    QString::fromStdString((*itr)->look_at().to_string()));

        position_elem.appendChild(pos);
        look_at_elem.appendChild(lkat);

        camera_elem.appendChild(position_elem);
        camera_elem.appendChild(look_at_elem);

        scene_elem.appendChild(camera_elem);
    }

    for (auto itr = _scene->lights().begin(); itr < _scene->lights().end(); itr++)
    {
        QDomElement light_elem = scene_doc.createElement("light");
        scene_elem.appendChild(light_elem);

        QDomElement position_elem = scene_doc.createElement("position");
        scene_elem.appendChild(position_elem);

        QDomElement color_elem = scene_doc.createElement("color");
        scene_elem.appendChild(color_elem);

        QDomText pos = scene_doc.createTextNode(
                    QString::fromStdString((*itr)->position().to_string()));

        QDomText color = scene_doc.createTextNode(
                    QString::fromStdString((*itr)->color().to_string()));

        position_elem.appendChild(pos);
        color_elem.appendChild(color);

        light_elem.appendChild(position_elem);
        light_elem.appendChild(color_elem);

        scene_elem.appendChild(light_elem);
    }

    for (auto itr = _scene->shapes().begin(); itr < _scene->shapes().end(); itr++)
        serialize(scene_doc, scene_elem, *itr);

    QFile file(QString::fromStdString(output_path));
    if (file.open(QFile::WriteOnly))
    {
        QTextStream out(&file);
        out << scene_doc << endl;
        file.close();
    }
    else
    {
        std::cerr << "error: cannot write the scene file "
                  << output_path << std::endl;
        exit(1);
    }
}

Scene* Serializer::read_from_XML(const std::string &input_path)
{
    QDomDocument xml_document;
    QFile file(QString::fromStdString(input_path));
    if (!file.open(QFile::ReadOnly))
    {
        std::cerr << "error: cannot open the input scene file "
                  << input_path << std::endl;
        return nullptr;
    }

    xml_document.setContent(&file);
    file.close();

    QDomElement scene_elem      { xml_document.firstChildElement("scene")   };
    QDomElement settings_elem   { scene_elem.firstChildElement("settings")  };
    QDomElement path_elem       { settings_elem.firstChildElement("path")   };
    QDomElement xres_elem       { settings_elem.firstChildElement("xres")   };
    QDomElement yres_elem       { settings_elem.firstChildElement("yres")   };
    QDomElement mode_elem       { settings_elem.firstChildElement("mode")   };
    QDomElement depth_elem      { settings_elem.firstChildElement("depth")  };
    QDomElement sample_elem     { settings_elem.firstChildElement("sampl")  };
    QDomElement illum_elem      { settings_elem.firstChildElement("illum")  };

    _scene->_output_img_path        = path_elem.text().toStdString();
    _scene->_output_img->width()    = xres_elem.text().toUInt();
    _scene->_output_img->height()   = yres_elem.text().toUInt();
    _scene->_output_img->init();
    _scene->_mode                   = mode_elem.text().toStdString();
    _scene->max_depth()             = depth_elem.text().toUInt();
    _scene->nb_samples()            = sqrt(sample_elem.text().toUInt());
    (*_scene->base_illuminant_SPD_p())=
            Spectra::spectrum(illum_elem.text().toStdString());

    populate_scene_from_XML(scene_elem);

    return _scene;
}

void Serializer::populate_scene_from_XML(const QDomElement &scene_elem)
{
    QDomElement obj_elem = scene_elem.firstChildElement("camera");

    while (!obj_elem.isNull())
    {
        if (obj_elem.tagName() == "camera")
            add_camera(obj_elem);
        else
            if (obj_elem.tagName() == "light")
                add_light(obj_elem);
            else
                if (obj_elem.tagName() == "plane")
                    add_shape(obj_elem, false);
                else
                    if (obj_elem.tagName() == "sphere")
                        add_shape(obj_elem, true);

        obj_elem = obj_elem.nextSiblingElement();
    }
}

void Serializer::add_camera(QDomElement &cam_elem)
{
    QDomElement pos_elem    = cam_elem.firstChildElement("position");
    QDomElement lkat_elem   = cam_elem.firstChildElement("look_at");

    QStringList pos_str = pos_elem.text().split(", ");
    Vec3d pos(pos_str[0].toDouble(),
              pos_str[1].toDouble(),
              pos_str[2].toDouble());

    QStringList lkat_str = lkat_elem.text().split(", ");
    Vec3d lkat(lkat_str[0].toDouble(),
               lkat_str[1].toDouble(),
               lkat_str[2].toDouble());

    _scene->add(new Camera(pos, lkat, _scene->_output_img->aspect_ratio()));
}

void Serializer::add_light(QDomElement &light_elem)
{
    QDomElement pos_elem    { light_elem.firstChildElement("position")  };
    QDomElement color_elem  { light_elem.firstChildElement("color")     };

    QStringList pos_str { pos_elem.text().split(", ") };
    Vec3d pos(pos_str[0].toDouble(),
              pos_str[1].toDouble(),
              pos_str[2].toDouble());

    QStringList color_str { color_elem.text().split(", ") };
    Color color(color_str[0].toDouble(),
                color_str[1].toDouble(),
                color_str[2].toDouble(),
                color_str[3].toDouble());

    _scene->add(new Light(20, pos, color));
}

void Serializer::add_shape(QDomElement &shape_elem, bool sphere)
{
    QDomElement center_elem;
    QDomElement radius_elem;

    if (sphere)
    {
        center_elem = shape_elem.firstChildElement("center");
        radius_elem = shape_elem.firstChildElement("radius");
    }
    else
    {
        center_elem = shape_elem.firstChildElement("normal");
        radius_elem = shape_elem.firstChildElement("distance");
    }

    QDomElement color_elem      { shape_elem.firstChildElement("color")       };
    QDomElement mat_elem        { shape_elem.firstChildElement("material")    };
    QDomElement refl_elem       { shape_elem.firstChildElement("reflectance") };
    QDomElement emission_elem   { shape_elem.firstChildElement("emission")    };
    QDomElement fluo_elem       { shape_elem.firstChildElement("fluorescence")};

    QStringList center_str { center_elem.text().split(", ") };
    Vec3d center(center_str[0].toDouble(),
                 center_str[1].toDouble(),
                 center_str[2].toDouble());

    double radius { radius_elem.text().toDouble() };

    if (_scene->mode() == "rt") // Ray tracing
    {
        QStringList color_str { color_elem.text().split(", ") };
        Color color(color_str[0].toDouble(),
                    color_str[1].toDouble(),
                    color_str[2].toDouble(),
                    color_str[3].toDouble());

        double emission { emission_elem.text().toDouble() };

        const Material *material {
            Materials::material(mat_elem.text().toStdString()) };

        if (sphere)
            _scene->add(new Sphere(center, radius, color, material, emission));
        else
            _scene->add(new Plane(center, radius, color, material, emission));
    }
    else    // Monte Carlo Path Tracing
    {
        const Fluorescence *fluo { fluo_elem.isNull() ?
                        nullptr : read_fluorescence_component(fluo_elem) };

        const Spectrum<> *emission_spctr {
            Spectra::spectrum(emission_elem.text().toStdString()) };

        const Spectrum<> *refl_spctr {
            Spectra::spectrum(refl_elem.text().toStdString()) };

        MaterialPBR *material_pbr {
            MaterialsPBR::material(mat_elem.text().toStdString()) };

        material_pbr->set_fluorescence(fluo);

        if (refl_spctr != nullptr)
            material_pbr->set_reflectance(refl_spctr);
        else
        {
            double emission { refl_elem.text().toDouble() };
            material_pbr->set_reflectance(new ConstantSPD<>(emission));
        }

        if (emission_spctr != nullptr)
        {
            if (sphere)
                _scene->add(new Sphere(center, radius,
                                       material_pbr, emission_spctr));
            else
                _scene->add(new Plane(center, radius, material_pbr,
                                      emission_spctr));
        }
        else
        {
            double emission { emission_elem.text().toDouble() };

            if (_scene->mode() == "mcpt") // Ray tracing
            {
                QStringList color_str { color_elem.text().split(", ") };
                const Color color(color_str[0].toDouble(),
                        color_str[1].toDouble(),
                        color_str[2].toDouble(),
                        color_str[3].toDouble());

                if (sphere)
                    _scene->add(new Sphere(center, radius, color, material_pbr, emission));
                else
                    _scene->add(new Plane(center, radius, color, material_pbr, emission));
            }
            else
            {
                if (sphere)
                    _scene->add(new Sphere(center, radius, material_pbr, emission));
                else
                    _scene->add(new Plane(center, radius, material_pbr, emission));
            }
        }
    }
}

const Fluorescence* Serializer::read_fluorescence_component(QDomElement &fluo_elem)
{
    QDomElement absorption_elem { fluo_elem.firstChildElement("absorption") };
    QDomElement stokes_elem     { fluo_elem.firstChildElement("stokes")     };

    if (absorption_elem.isNull())
    {
        std::cerr << "error: fluorescent shape has no absorption spectrum"
                  << std::endl;
        return nullptr;
    }

    if (stokes_elem.isNull())
    {
        std::cerr << "error: fluorescent shape has no stokes shift field"
                  << std::endl;
        return nullptr;
    }

    std::cout << "BLBLBLBLBL :: " << "a" + absorption_elem.text().toStdString()
              << std::endl;

    const Spectrum<> *absorption_spectrum {
        Spectra::spectrum("a" + absorption_elem.text().toStdString()) };

    return new Fluorescence(absorption_spectrum, stokes_elem.text().toUInt());
}

void Serializer::serialize(QDomDocument &scene_doc,
                           QDomElement &scene_elem, Shape* s)
{
    Sphere* sphere  { dynamic_cast<Sphere*>(s)   };
    Plane*  plane   { dynamic_cast<Plane*>(s)    };

    if (sphere)
        serialize(scene_doc, scene_elem, sphere);

    if (plane)
        serialize(scene_doc, scene_elem, plane);
}

void Serializer::serialize(QDomDocument &scene_doc,
                           QDomElement &scene_elem, Sphere* s)
{
    QDomElement shape_elem = scene_doc.createElement("sphere");
    scene_elem.appendChild(shape_elem);

    QDomElement center_elem = scene_doc.createElement("center");
    shape_elem.appendChild(center_elem);

    QDomText center = scene_doc.createTextNode(
                QString::fromStdString(s->position().to_string()));

    QDomElement radius_elem = scene_doc.createElement("radius");
    shape_elem.appendChild(radius_elem);

    QDomText radius = scene_doc.createTextNode(
                QString::number(s->radius()));

    QDomElement color_elem = scene_doc.createElement("color");
    shape_elem.appendChild(color_elem);

    QDomText color = scene_doc.createTextNode(
                QString::fromStdString(s->color().to_string()));

    QDomElement material_elem = scene_doc.createElement("material");
    shape_elem.appendChild(material_elem);

    QDomText material = scene_doc.createTextNode(
                QString::fromStdString(s->material()->name()));

    QDomElement emission_elem = scene_doc.createElement("emission");
    shape_elem.appendChild(emission_elem);

    QDomText emission = scene_doc.createTextNode(
                QString::number(s->emission()));

    center_elem.appendChild(center);
    radius_elem.appendChild(radius);
    color_elem.appendChild(color);
    material_elem.appendChild(material);
    emission_elem.appendChild(emission);

    shape_elem.appendChild(center_elem);
    shape_elem.appendChild(radius_elem);
    shape_elem.appendChild(color_elem);
    shape_elem.appendChild(material_elem);
    shape_elem.appendChild(emission_elem);
}

void Serializer::serialize(QDomDocument &scene_doc,
                           QDomElement &scene_elem, Plane* p)
{
    QDomElement shape_elem = scene_doc.createElement("plane");
    scene_elem.appendChild(shape_elem);

    QDomElement normal_elem = scene_doc.createElement("normal");
    shape_elem.appendChild(normal_elem);

    QDomText normal = scene_doc.createTextNode(
                QString::fromStdString(p->normal().to_string()));

    QDomElement distance_elem = scene_doc.createElement("distance");
    shape_elem.appendChild(distance_elem);

    QDomText distance = scene_doc.createTextNode(
                QString::number(p->distance()));

    QDomElement color_elem = scene_doc.createElement("color");
    shape_elem.appendChild(color_elem);

    QDomText color = scene_doc.createTextNode(
                QString::fromStdString(p->color().to_string()));

    QDomElement material_elem = scene_doc.createElement("material");
    shape_elem.appendChild(material_elem);

    QDomText material = scene_doc.createTextNode(
                QString::fromStdString(p->material()->name()));

    normal_elem.appendChild(normal);
    distance_elem.appendChild(distance);
    color_elem.appendChild(color);
    material_elem.appendChild(material);

    shape_elem.appendChild(normal_elem);
    shape_elem.appendChild(distance_elem);
    shape_elem.appendChild(color_elem);
    shape_elem.appendChild(material_elem);

    scene_elem.appendChild(shape_elem);
}
