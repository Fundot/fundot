/**
 * MIT License

 * Copyright (c) 2020 Jiacheng Huang

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FUNDOT_OBJECT_H
#define FUNDOT_OBJECT_H

#include <any>
#include <cstddef>

namespace fundot {
class Object {
public:
    Object() = default;

    Object(const Object& other) = default;

    template<typename T>
    Object(const T& other)
    {
        obj_ = other;
    }

    Object& operator=(const Object& other) = default;

    template<typename T>
    Object& operator=(const T& other)
    {
        obj_ = other;
        return *this;
    }

    template<typename T>
    explicit operator T() const
    {
        return std::any_cast<T>(obj_);
    }

    template<typename T>
    bool hasType() const
    {
        return obj_.type() == typeid(T);
    }

    const std::type_info& type() const { return obj_.type(); }

private:
    std::any obj_;
};

}  // namespace fundot

#endif