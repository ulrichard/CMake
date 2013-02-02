/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2012 Stephen Kelly <steveire@gmail.com>

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/

#include "cmGeneratorExpressionDAGChecker.h"

#include "cmMakefile.h"

//----------------------------------------------------------------------------
cmGeneratorExpressionDAGChecker::cmGeneratorExpressionDAGChecker(
                const cmListFileBacktrace &backtrace,
                const std::string &target,
                const std::string &property,
                const GeneratorExpressionContent *content,
                cmGeneratorExpressionDAGChecker *parent)
  : Parent(parent), Target(target), Property(property),
    Content(content), Backtrace(backtrace)
{
  const cmGeneratorExpressionDAGChecker *top = this;
  const cmGeneratorExpressionDAGChecker *p = this->Parent;
  while (p)
    {
    top = p;
    p = p->Parent;
    }
  this->CheckResult = this->checkGraph();

  if (CheckResult == DAG && (top->Property == "INCLUDE_DIRECTORIES"
      || top->Property == "COMPILE_DEFINITIONS") )
    {
    std::map<cmStdString, std::set<cmStdString> >::const_iterator it
                                                    = top->Seen.find(target);
    if (it != top->Seen.end()
        && it->second.find(property) != it->second.end())
      {
      this->CheckResult = ALREADY_SEEN;
      return;
      }
    else
      {
      const_cast<cmGeneratorExpressionDAGChecker *>(top)
                                              ->Seen[target].insert(property);
      }
    }
}

//----------------------------------------------------------------------------
cmGeneratorExpressionDAGChecker::Result
cmGeneratorExpressionDAGChecker::check() const
{
  return this->CheckResult;
}

//----------------------------------------------------------------------------
void cmGeneratorExpressionDAGChecker::reportError(
                  cmGeneratorExpressionContext *context,
                  const std::string &expr)
{
  if (this->CheckResult == DAG)
    {
    return;
    }

  context->HadError = true;
  if (context->Quiet)
    {
    return;
    }

  const cmGeneratorExpressionDAGChecker *parent = this->Parent;

  if (parent && !parent->Parent)
    {
    cmOStringStream e;
    e << "Error evaluating generator expression:\n"
      << "  " << expr << "\n"
      << "Self reference on target \""
      << context->HeadTarget->GetName() << "\".\n";
    context->Makefile->GetCMakeInstance()
      ->IssueMessage(cmake::FATAL_ERROR, e.str().c_str(),
                      parent->Backtrace);
    return;
    }

  {
  cmOStringStream e;
  e << "Error evaluating generator expression:\n"
    << "  " << expr << "\n"
    << "Dependency loop found.";
  context->Makefile->GetCMakeInstance()
    ->IssueMessage(cmake::FATAL_ERROR, e.str().c_str(),
                    context->Backtrace);
  }

  int loopStep = 1;
  while (parent)
    {
    cmOStringStream e;
    e << "Loop step " << loopStep << "\n"
      << "  "
      << (parent->Content ? parent->Content->GetOriginalExpression() : expr)
      << "\n";
    context->Makefile->GetCMakeInstance()
      ->IssueMessage(cmake::FATAL_ERROR, e.str().c_str(),
                      parent->Backtrace);
    parent = parent->Parent;
    ++loopStep;
    }
}

//----------------------------------------------------------------------------
cmGeneratorExpressionDAGChecker::Result
cmGeneratorExpressionDAGChecker::checkGraph() const
{
  const cmGeneratorExpressionDAGChecker *parent = this->Parent;
  while (parent)
    {
    if (this->Target == parent->Target && this->Property == parent->Property)
      {
      return parent->Parent ? CYCLIC_REFERENCE : SELF_REFERENCE;
      }
    parent = parent->Parent;
    }
  return DAG;
}

//----------------------------------------------------------------------------
bool cmGeneratorExpressionDAGChecker::EvaluatingLinkLibraries()
{
  const cmGeneratorExpressionDAGChecker *top = this;
  const cmGeneratorExpressionDAGChecker *parent = this->Parent;
  while (parent)
    {
    top = parent;
    parent = parent->Parent;
    }

  const char *prop = top->Property.c_str();
  return (strcmp(prop, "LINK_LIBRARIES") == 0
       || strcmp(prop, "LINK_INTERFACE_LIBRARIES") == 0
       || strcmp(prop, "IMPORTED_LINK_INTERFACE_LIBRARIES") == 0
       || strncmp(prop, "LINK_INTERFACE_LIBRARIES_", 26) == 0
       || strncmp(prop, "IMPORTED_LINK_INTERFACE_LIBRARIES_", 35) == 0);
}

//----------------------------------------------------------------------------
bool cmGeneratorExpressionDAGChecker::EvaluatingIncludeDirectories()
{
  const char *prop = this->Property.c_str();
  return (strcmp(prop, "INCLUDE_DIRECTORIES") == 0
       || strcmp(prop, "INTERFACE_INCLUDE_DIRECTORIES") == 0 );
}

//----------------------------------------------------------------------------
bool cmGeneratorExpressionDAGChecker::EvaluatingCompileDefinitions()
{
  const char *prop = this->Property.c_str();
  return (strcmp(prop, "COMPILE_DEFINITIONS") == 0
       || strcmp(prop, "INTERFACE_COMPILE_DEFINITIONS") == 0 );
}
