#***************************************************************************
#*                                                                         *
#*   Copyright (c) 2013 - Juergen Riegel <FreeCAD@juergen-riegel.net>      *  
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU Lesser General Public License (LGPL)    *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with this program; if not, write to the Free Software   *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************

__title__="Mechanical Analysis managment"
__author__ = "Juergen Riegel"
__url__ = "http://www.freecadweb.org"

    
def colorValue(value,min,max):
    'creates a homogenouse color ramp between [min,max] green to red'
    if value < min: return (0.0,1.0,0.0)    
    if value > max: return (1.0,0.0,0.0)
    if value < (min + (max-min)/2.0):
        return ((value-min) / ((max-min)/2.0),1.0,0.0)
    else:
        return (1.0,1-((value-min-((max-min)/2.0)) / ((max-min)/2.0)),0.0)
    