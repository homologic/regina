
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "angle/nanglestructurelist.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"
#include "engine/angle/NJNIAngleStructureList.h"

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_allowsStrict
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAngleStructureList, me)->allowsStrict();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_allowsTaut
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAngleStructureList, me)->allowsTaut();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_getNumberOfStructures
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAngleStructureList, me)->
        getNumberOfStructures();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_getStructure
        (JNIEnv* env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NAngleStructure*)GET_ENGINE_OBJECT(env, NAngleStructureList,
        me)->getStructure(index),
        "normal/engine/implementation/jni/angle/NJNIAngleStructure");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_getTriangulation
        (JNIEnv* env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env,
        NAngleStructureList, me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT void JNICALL Java_normal_engine_implementation_jni_angle_NJNIAngleStructureList_newNAngleStructureList
        (JNIEnv* env, jobject me, jobject triangulation) {
    ASSIGN_ENGINE_OBJECT(env, new NAngleStructureList(
        GET_ENGINE_OBJECT(env, NTriangulation, triangulation)), me);
}

