//
//  object.cpp
//  gni
//
//  Created by John Othwolo on 3/30/25.
//

#include <stdio.h>
#include "gni/gni.hpp"

gni::Object::Object(jobject object)
{
    if (!object)
        return;
    object_ = gni::GniCore::GetInstance()->GetJniEnv()->NewGlobalRef(object);
}

gni::Object::Object(Object&& object)
{
    SetImpl(object.GetImpl());
    object.SetImpl(NULL);
}

gni::Object::~Object()
{
    gni::GniCore::GetInstance()->GetJniEnv()->DeleteGlobalRef(GetImpl());
}
