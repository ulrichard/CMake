/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmRegexTools.h"
#include "cmMakefile.h"

#include <cmsys/RegularExpression.hxx>

#include <vector>

//----------------------------------------------------------------------------
namespace
{
  class RegexReplacement
  {
  public:
    RegexReplacement(const char* s): number(-1), value(s) {}
    RegexReplacement(const std::string& s): number(-1), value(s) {}
    RegexReplacement(int n): number(n), value() {}
    RegexReplacement() {}
    int number;
    std::string value;
  };
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
RegexReplacer::RegexReplacer(std::string const& regex, std::string const& replace)
    : Regex(regex)
    , Replace(replace)
    , ErrorCallback(NULL)
{

}
//----------------------------------------------------------------------------
void RegexReplacer::SetErrorReportingCallBack(BaseCallBack* cb)
{
    ErrorCallback = cb;
}
//----------------------------------------------------------------------------
void RegexReplacer::SetMakefile(cmMakefile* mf)
{
    Makefile = mf;
}
//----------------------------------------------------------------------------
void RegexReplacer::SetError(std::string const& err) const
{
    if(ErrorCallback != NULL)
        (*ErrorCallback)(err);
}
//----------------------------------------------------------------------------
std::string RegexReplacer::operator()(std::string const& input) const
{
  // Pull apart the replace expression to find the escaped [0-9] values.
  std::vector<RegexReplacement> replacement;
  std::string::size_type l = 0;
  while(l < this->Replace.length())
    {
    std::string::size_type r = this->Replace.find("\\", l);
    if(r == std::string::npos)
      {
      r = this->Replace.length();
      replacement.push_back(this->Replace.substr(l, r-l));
      }
    else
      {
      if(r-l > 0)
        {
        replacement.push_back(this->Replace.substr(l, r-l));
        }
      if(r == (this->Replace.length()-1))
        {
        this->SetError("sub-command REGEX, mode REPLACE: "
                       "replace-expression ends in a backslash.");
        return input;
        }
      if((this->Replace[r+1] >= '0') && (this->Replace[r+1] <= '9'))
        {
        replacement.push_back(this->Replace[r+1]-'0');
        }
      else if(this->Replace[r+1] == 'n')
        {
        replacement.push_back("\n");
        }
      else if(this->Replace[r+1] == '\\')
        {
        replacement.push_back("\\");
        }
      else
        {
        std::string e = "sub-command REGEX, mode REPLACE: Unknown escape \"";
        e += this->Replace.substr(r, 2);
        e += "\" in replace-expression.";
        this->SetError(e);
        return input;
        }
      r += 2;
      }
    l = r;
    }

  // Compile the regular expression.
  cmsys::RegularExpression re;
  if(!re.compile(this->Regex.c_str()))
    {
    std::string e =
      "sub-command REGEX, mode REPLACE failed to compile regex \""+
      this->Regex+"\".";
    this->SetError(e);
    return input;
    }

  // Scan through the input for all matches.
  std::string output;
  std::string::size_type base = 0;
  while(re.find(input.c_str()+base))
    {
    if(this->Makefile != NULL)
        this->Makefile->StoreMatches(re);
    std::string::size_type l2 = re.start();
    std::string::size_type r = re.end();

    // Concatenate the part of the input that was not matched.
    output += input.substr(base, l2);

    // Make sure the match had some text.
    if(r-l2 == 0)
      {
      std::string e = "sub-command REGEX, mode REPLACE regex \""+
        this->Regex+"\" matched an empty string.";
      this->SetError(e);
      return input;
      }

    // Concatenate the replacement for the match.
    for(unsigned int i=0; i < replacement.size(); ++i)
      {
      if(replacement[i].number < 0)
        {
        // This is just a plain-text part of the replacement.
        output += replacement[i].value;
        }
      else
        {
        // Replace with part of the match.
        int n = replacement[i].number;
        std::string::size_type start = re.start(n);
        std::string::size_type end = re.end(n);
        std::string::size_type len = input.length()-base;
        if((start != std::string::npos) && (end != std::string::npos) &&
           (start <= len) && (end <= len))
          {
          output += input.substr(base+start, end-start);
          }
        else
          {
          std::string e =
            "sub-command REGEX, mode REPLACE: replace expression \""+
            this->Replace+"\" contains an out-of-range escape for regex \""+
            this->Regex+"\".";
          this->SetError(e);
          return input;
          }
        }
      }

    // Move past the match.
    base += r;
    }

  // Concatenate the text after the last match.
  output += input.substr(base, input.length()-base);

  return output;
}
