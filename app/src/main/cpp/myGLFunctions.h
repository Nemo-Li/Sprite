/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef MY_GL_FUNCTIONS_H
#define MY_GL_FUNCTIONS_H

#include <stdio.h>
#include <string>
#include <GLES3/gl32.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

void MyGLInits();
void CheckGLError(std::string functionName);

#endif //MY_GL_FUNCTIONS_H
