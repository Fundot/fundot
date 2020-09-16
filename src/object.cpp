#include "../include/object.h"

namespace fundot
{
    istream &operator>>(istream &is, Object &obj)
    {
        obj = obj._scan(is);
        return is;
    }

    ostream &operator<<(ostream &os, const Object &obj)
    {
        obj._print(os);
        return os;
    }

    Object &Object::operator[](size_t index)
    {
        if (_value.type() == typeid(vector<Object>))
        {
            return value<vector<Object>>()[index];
        }
        else
        {
        }
    }

    Object &Object::operator[](Identifier id)
    {
        if (_value.type() == typeid(map<Identifier, Object>))
        {
            return value<map<Identifier, Object>>()[id];
        }
        else
        {
        }
    }

    Object Object::_scan(istream &is)
    {
        char c;
        is >> c;
        if (c == '"')
        {
            return _scanString(is);
        }
        else if (c == '-' || (c >= '0' && c <= '9'))
        {
            is.putback(c);
            return _scanNumber(is);
        }
        else if (c == '[')
        {
            return _scanVector(is);
        }
        else if (c == '{')
        {
            return _scanMap(is);
        }
        else if (c == '(')
        {
            return _scanList(is);
        }
        else if (_separators.find(c) != _separators.end())
        {
            return Object(c);
        }
        else
        {
            is.putback(c);
            return _scanIdentifier(is);
        }

        return Object(c);
    }

    Object Object::_scanNumber(istream &is)
    {
        double double_num;
        is >> double_num;
        int int_num = static_cast<int>(double_num);
        if (double_num - int_num == 0)
        {
            return Object(int_num);
        }
        return Object(double_num);
    }

    Object Object::_scanString(istream &is)
    {
        is >> noskipws;
        string str;
        char c;
        while (is >> c)
        {
            if (c == '"')
            {
                break;
            }
            else if (c == '\\')
            {
                is >> c;
                if (c == 'b')
                {
                    c = '\b';
                }
                else if (c == 'f')
                {
                    c = '\f';
                }
                else if (c == 'n')
                {
                    c = '\n';
                }
                else if (c == 'r')
                {
                    c = '\r';
                }
                else if (c == 't')
                {
                    c = '\t';
                }
                else if (c == 'u')
                {
                    for (size_t i = 0; i < 4; ++i)
                    {
                        is >> c;
                        if (i != 3)
                        {
                            str.push_back(c);
                        }
                    }
                }
            }
            else if (isspace(c) && c != ' ')
            {
                continue;
            }
            str.push_back(c);
        }
        Object obj(str);
        is >> skipws;
        return obj;
    }

    Object Object::_scanVector(istream &is)
    {
        vector<Object> obj_vct;
        Object obj(_scan(is));
        char c;
        while ((obj.type() == typeid(char) && obj.value<char>() == ']') == false)
        {
            obj_vct.push_back(obj);
            is >> c;
            if (c == ']')
            {
                break;
            }
            obj = _scan(is);
        }
        return Object(obj_vct);
    }

    Object Object::_scanMap(istream &is)
    {
        map<Identifier, Object> obj_map;
        Object obj(_scan(is));
        char c;
        while ((obj.type() == typeid(char) && obj.value<char>() == '}') == false)
        {
            is >> c;
            if (c != ':')
            {
                // error handle
            }
            obj_map[obj.value<Identifier>()] = _scan(is);
            is >> c;
            if (c == '}')
            {
                break;
            }
            obj = _scan(is);
        }
        return Object(obj_map);
    }

    Object Object::_scanIdentifier(istream &is)
    {
        Identifier id;
        char c;
        is >> noskipws;
        while (is >> c)
        {
            if (_separators.find(c) != _separators.end())
            {
                is.putback(c);
                break;
            }
            else if (isspace(c))
            {
                break;
            }
            id.push_back(c);
        }
        is >> skipws;
        return Object(id);
    }

    Object Object::_scanList(istream &is)
    {
        list<Object> obj_lst;
        Object obj(_scan(is));
        while ((obj.type() == typeid(char) && obj.value<char>() == ')') == false)
        {
            obj_lst.push_back(obj);
            obj = _scan(is);
        }
        return Object(obj_lst);
    }

    void Object::_print(ostream &os) const
    {
        if (_value.type() == typeid(string))
        {
            _printString(os);
        }
        else if (_value.type() == typeid(int) || _value.type() == typeid(double))
        {
            _printNumber(os);
        }
        else if (_value.type() == typeid(vector<Object>))
        {
            _printVector(os);
        }
        else if (_value.type() == typeid(map<Identifier, Object>))
        {
            _printMap(os);
        }
        else if (_value.type() == typeid(Identifier))
        {
            _printIdentifier(os);
        }
        else if (_value.type() == typeid(list<Object>))
        {
            _printList(os);
        }
    }

    void Object::_printNumber(ostream &os) const
    {
        if (_value.type() == typeid(int))
        {
            os << any_cast<int>(_value);
        }
        else if (_value.type() == typeid(double))
        {
            os << any_cast<double>(_value);
        }
    }

    void Object::_printString(ostream &os) const
    {
        os << '"' << any_cast<string>(_value) << '"';
    }

    void Object::_printVector(ostream &os) const
    {
        os << '[';
        vector<Object> obj_vct = any_cast<vector<Object>>(_value);
        vector<Object>::iterator it = obj_vct.begin();
        while (it != obj_vct.end())
        {
            os << *it++;
            if (it != obj_vct.end())
            {
                os << ", ";
            }
        }
        os << ']';
    }

    void Object::_printMap(ostream &os) const
    {
        os << '{';
        map<Identifier, Object> obj_map = any_cast<map<Identifier, Object>>(_value);
        map<Identifier, Object>::iterator it = obj_map.begin();
        while (it != obj_map.end())
        {
            os << it->first << ": " << it->second;
            ++it;
            if (it != obj_map.end())
            {
                os << ", ";
            }
        }
        os << '}';
    }

    void Object::_printIdentifier(ostream &os) const
    {
        os << any_cast<Identifier>(_value);
    }

    void Object::_printList(ostream &os) const
    {
        os << '(';
        list<Object> obj_lst = any_cast<list<Object>>(_value);
        list<Object>::iterator it = obj_lst.begin();
        while (it != obj_lst.end())
        {

            os << *it++;
            if (it != obj_lst.end())
            {
                os << ' ';
            }
        }
        os << ')';
    }

} // namespace fundot