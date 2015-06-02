/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef cmRegexTools_h
#define cmRegexTools_h

#include <string>

class cmMakefile;

class BaseCallBack
{
public:
  virtual void operator()(const std::string&) = 0;
  virtual ~BaseCallBack() {}
};

template<class ClassT>
class RegexReplacerCallback : public BaseCallBack
{
public:
  typedef void(ClassT::* FuncT)(const std::string&);

  RegexReplacerCallback(ClassT* c, FuncT fn)
    : _fn(fn), _c(c) {}

  void operator()(const std::string& err)
  {
    return (_c->*_fn)(err);
  }

private:
  FuncT _fn;
  ClassT* _c;
};

class RegexReplacer
{
public:
    RegexReplacer(std::string const& regex, std::string const& replace);

    void SetErrorReportingCallBack(BaseCallBack* cb);
    void SetMakefile(cmMakefile* mf);

    std::string operator()(std::string const& input) const;

private:
    void SetError(std::string const& err) const;

    std::string     Regex;
    std::string     Replace;
    BaseCallBack*   ErrorCallback;
    cmMakefile*     Makefile;
};


#endif
