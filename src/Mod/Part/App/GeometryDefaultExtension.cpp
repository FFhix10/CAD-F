/***************************************************************************
 *   Copyright (c) 2019 Abdullah Tahiri <abdullah.tahiri.yo@gmail.com>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#include <Base/Writer.h>
#include <Base/Reader.h>
#include <Base/Tools.h>
#include <Base/Exception.h>

#include "GeometryDefaultExtension.h"

#include "GeometryStringExtensionPy.h"
#include "GeometryIntExtensionPy.h"
#include "GeometryBoolExtensionPy.h"
#include "GeometryDoubleExtensionPy.h"

using namespace Part;

//---------- Geometry Extension
template <typename T>
GeometryDefaultExtension<T>::GeometryDefaultExtension(const T& val, std::string name):value(val)
{
    setName(name);
}

// Persistence implementer
template <typename T>
unsigned int GeometryDefaultExtension<T>::getMemSize (void) const
{
    return 1;
}

template <typename T>
void GeometryDefaultExtension<T>::Save(Base::Writer &writer) const
{

    writer.Stream() << writer.ind() << "<GeoExtension type=\"" << this->getTypeId().getName();

    const std::string name = getName();

    if(name.size() > 0)
        writer.Stream() << "\" name=\"" << name;

    writer.Stream() << "\" value=\"" << value << "\"/>" << std::endl;
}

template <typename T>
void GeometryDefaultExtension<T>::Restore(Base::XMLReader &reader)
{
    restoreNameAttribute(reader);

    value = reader.getAttribute("value");
}

template <typename T>
std::unique_ptr<Part::GeometryExtension> GeometryDefaultExtension<T>::copy(void) const
{
    std::unique_ptr<GeometryDefaultExtension<T>> cpy = std::make_unique<GeometryDefaultExtension<T>>();

    cpy->value = this->value;
    cpy->setName(this->getName());

    #if (defined(__GNUC__) && __GNUC__ < 7 ) || defined(_MSC_VER)
        return std::move(cpy); // GCC 4.8 and MSC do not support automatic move constructor call if the compiler fails to elide
    #else
        return cpy; // all the others do automatic move constructor if RVO optimization not possible.
    #endif
    // Advise from Scott Meyers Effective Modern c++:
    //
    // Don't std::move(cpy); RVO optimization Item 25, if the compiler fails to elide, would have to move it anyway
    // move constructor is executed if available (it is). Unique_ptr does not have copy constructor.
    //
    // That would work perfectly with GCC 7.3.0. However, GCC 4.8.4 misserably fails:
    //
    // /home/travis/build/FreeCAD/FreeCAD/src/Mod/Part/App/GeometryDefaultExtension.cpp: In instantiation of
    // ???std::unique_ptr<Part::GeometryExtension> Part::GeometryDefaultExtension<T>::copy() const [with T = long int]???:
    // /home/travis/build/FreeCAD/FreeCAD/src/Mod/Part/App/GeometryDefaultExtension.cpp:164:16:   required from here
    // /home/travis/build/FreeCAD/FreeCAD/src/Mod/Part/App/GeometryDefaultExtension.cpp:84:12: error: cannot bind //
    // ???std::unique_ptr<Part::GeometryDefaultExtension<long int>, std::default_delete<Part::GeometryDefaultExtension<long int> > >??? lvalue
    // to ???std::unique_ptr<Part::GeometryDefaultExtension<long int>, std::default_delete<Part::GeometryDefaultExtension<long int> > >&&???
}

template <typename T>
PyObject * GeometryDefaultExtension<T>::getPyObject(void)
{
    THROWM(Base::NotImplementedError,"Python object not implemented for default geometry extension template type. Template Specialisation missing."); // use template specialisation to provide the actual object
}

namespace Part {
// ----------------------------- Template specialisations----------------------------------------------------

//typedef Part::GeometryDefaultExtension<long> GeometryIntExtension;
//typedef Part::GeometryStringExtension<std::string> GeometryStringExtension;

// ---------- GeometryIntExtension ----------
TYPESYSTEM_SOURCE_TEMPLATE_T(Part::GeometryIntExtension,Part::GeometryExtension)

template <>
PyObject * GeometryDefaultExtension<long>::getPyObject(void)
{
    return new GeometryIntExtensionPy(new GeometryIntExtension(*this));
}

template <>
void GeometryDefaultExtension<long>::Restore(Base::XMLReader &reader)
{
    restoreNameAttribute(reader);

    value = reader.getAttributeAsInteger("value");
}

// ---------- GeometryStringExtension ----------
TYPESYSTEM_SOURCE_TEMPLATE_T(Part::GeometryStringExtension,Part::GeometryExtension)

template <>
PyObject * GeometryDefaultExtension<std::string>::getPyObject(void)
{
    return new GeometryStringExtensionPy(new GeometryStringExtension(*this));
}

// ---------- GeometryBoolExtension ----------
TYPESYSTEM_SOURCE_TEMPLATE_T(Part::GeometryBoolExtension,Part::GeometryExtension)

template <>
PyObject * GeometryDefaultExtension<bool>::getPyObject(void)
{
    return new GeometryBoolExtensionPy(new GeometryBoolExtension(*this));
}

template <>
void GeometryDefaultExtension<bool>::Restore(Base::XMLReader &reader)
{
    restoreNameAttribute(reader);

    value = (bool)reader.getAttributeAsInteger("value");
}

// ---------- GeometryDoubleExtension ----------
TYPESYSTEM_SOURCE_TEMPLATE_T(Part::GeometryDoubleExtension,Part::GeometryExtension)

template <>
PyObject * GeometryDefaultExtension<double>::getPyObject(void)
{
    return new GeometryDoubleExtensionPy(new GeometryDoubleExtension(*this));
}

template <>
void GeometryDefaultExtension<double>::Restore(Base::XMLReader &reader)
{
    restoreNameAttribute(reader);

    value = reader.getAttributeAsFloat("value");
}


// instantiate the types so that other translation units (python wrappers) can access template
//constructors other than the default.
template class GeometryDefaultExtension<long>;
template class GeometryDefaultExtension<std::string>;
template class GeometryDefaultExtension<bool>;
template class GeometryDefaultExtension<double>;


} //namespace Part
