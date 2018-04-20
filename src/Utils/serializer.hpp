#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include "Scene/scene.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include <QtXml/QDomElement>
#include <QFile>
#include <QTextStream>

class Serializer
{
public:
    void write_to_XML(std::string output_path)
    {
        QDomDocument scene_doc;
        const Image &img = **_scene.output_image_p();

        QDomElement scene_elem = scene_doc.createElement("scene");
        scene_doc.appendChild(scene_elem);

        QDomElement xres_elem   = scene_doc.createElement("xres");
        QDomElement yres_elem   = scene_doc.createElement("yres");
        QDomElement path_elem   = scene_doc.createElement("path");
        QDomElement depth_elem  = scene_doc.createElement("depth");

        QDomText xres   = scene_doc.createTextNode(QString::number(img.width()));
        QDomText yres   = scene_doc.createTextNode(QString::number(img.height()));
        QDomText depth  = scene_doc.createTextNode(QString::number(_scene.max_depth()));
        QDomText path   = scene_doc.createTextNode(QString::fromStdString(output_path));

        xres_elem.appendChild(xres);
        yres_elem.appendChild(yres);
        depth_elem.appendChild(depth);
        path_elem.appendChild(path);

        scene_elem.appendChild(xres_elem);
        scene_elem.appendChild(yres_elem);
        scene_elem.appendChild(path_elem);
        scene_elem.appendChild(depth_elem);


        for (auto itr = _scene.cameras().begin(); itr < _scene.cameras().end(); itr++)
        {
            QDomElement camera_elem = scene_doc.createElement("camera");
            scene_elem.appendChild(camera_elem);

            QDomText pos = scene_doc.createTextNode(
                        QString::fromStdString((*itr)->position().to_string()));

            QDomText lkat = scene_doc.createTextNode(
                        QString::fromStdString((*itr)->look_at().to_string()));

            camera_elem.appendChild(pos);
            camera_elem.appendChild(lkat);

            scene_elem.appendChild(camera_elem);
        }

        for (auto itr = _scene.lights().begin(); itr < _scene.lights().end(); itr++)
        {
            QDomElement light_elem = scene_doc.createElement("light");
            scene_elem.appendChild(light_elem);

            QDomText pos = scene_doc.createTextNode(
                        QString::fromStdString((*itr)->position().to_string()));

            QDomText color = scene_doc.createTextNode(
                        QString::fromStdString((*itr)->color().to_string()));

            light_elem.appendChild(pos);
            light_elem.appendChild(color);

            scene_elem.appendChild(light_elem);
        }

        for (auto itr = _scene.shapes().begin(); itr < _scene.shapes().end(); itr++)
        {
            QDomElement shape_elem = scene_doc.createElement("shape");
            scene_elem.appendChild(shape_elem);

            serialize(&scene_doc, &shape_elem, *itr);

            scene_elem.appendChild(shape_elem);
        }

        QFile file(QString::fromStdString(output_path));
        file.open(QFile::WriteOnly);

        QTextStream out(&file);
        out << scene_doc << endl;
        file.close();
    }

    Scene read_from_XML(std::string)
    {
        return _scene;
    }

    Scene& scene() { return _scene; }

private:
    void serialize(QDomDocument *scene_doc, QDomElement *shape_elem, Shape* s)
    {
        Sphere* sphere  { dynamic_cast<Sphere*>(s)   };
        Plane*  plane   { dynamic_cast<Plane*>(s)    };

        if (sphere)
        {
            serialize(scene_doc, shape_elem, sphere);
            return;
        }

        if (plane)
            serialize(scene_doc, shape_elem, plane);
    }

    void serialize(QDomDocument *scene_doc, QDomElement *shape_elem,
                           Sphere* s)
    {
        QDomText center = scene_doc->createTextNode(
                    QString::fromStdString(s->position().to_string()));

        QDomText radius = scene_doc->createTextNode(
                    QString::number(s->radius()));

        QDomText color = scene_doc->createTextNode(
                    QString::fromStdString(s->material().color().to_string()));

        QDomText material = scene_doc->createTextNode(
                    QString::fromStdString(s->material().name()));

        shape_elem->appendChild(center);
        shape_elem->appendChild(radius);
        shape_elem->appendChild(color);
        shape_elem->appendChild(material);
    }

    void serialize(QDomDocument *scene_doc, QDomElement *shape_elem,
                           Plane* p)
    {
        QDomText normal = scene_doc->createTextNode(
                    QString::fromStdString(p->normal().to_string()));

        QDomText dist   = scene_doc->createTextNode(
                    QString::number(p->distance()));

        QDomText color  = scene_doc->createTextNode(
                    QString::fromStdString(p->material().color().to_string()));

        QDomText material = scene_doc->createTextNode(
                    QString::fromStdString(p->material().name()));

        shape_elem->appendChild(normal);
        shape_elem->appendChild(dist);
        shape_elem->appendChild(color);
        shape_elem->appendChild(material);
    }

    Scene _scene;
};

#endif
