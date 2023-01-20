#pragma once
#include <chrono>
#include <functional>
#include <string>
#include <vector>

namespace SpeedTest
{
struct Settings
{
  unsigned minSize;
  unsigned maxSize;
  unsigned iterationCount;

  unsigned CalcStep() const { return (maxSize - minSize) / iterationCount; }
};

using namespace std::chrono;

template <class TestSubject>
struct Case
{
  std::string caseName;
  std::function<void(std::vector<TestSubject> &)> func;
};

struct IterationResult
{
  milliseconds elapsedTime;
//  TestSubject retValue;
};

struct CaseResult
{
  std::string caseName;
  std::vector<IterationResult> iterations;

  explicit CaseResult(std::string caseName) : caseName(caseName) {}
};

struct Results
{

 private:
  void OutputHeader(std::ostream &os) const
  {
    if (casesResults.empty()) return;

    os << "Size";
    for (const auto &caseResults : casesResults)
    {
      os << ", " << caseResults.caseName;
    }
  }

 public:
  std::vector<CaseResult> casesResults;
  Settings settings;

  friend std::ostream &operator<<(std::ostream &os, const Results &results)
  {
    results.OutputHeader(os);

    unsigned size = results.settings.minSize;
    unsigned step = results.settings.CalcStep();
    unsigned iterationCount = results.casesResults.at(0).iterations.size();

    for (unsigned i = 0; i < iterationCount; i++)
    {
      os << std::endl << size;

      for (const CaseResult &casesResult : results.casesResults)
      {
        os << ", " << casesResult.iterations.at(i).elapsedTime.count();
      }

      size += step;
    }

    return os;
  }

//  void OutputRetValues(std::ostream &os)
//  {
//    OutputHeader(os);
//
//    unsigned size = settings.minSize;
//    unsigned step = settings.CalcStep();
//    unsigned iterationCount = casesResults.at(0).iterations.size();
//
//    for (unsigned i = 0; i < iterationCount; i++)
//    {
//      os << size;
//
//      for (const CaseResult<TestSubject> &casesResult : casesResults)
//      {
//        os << ", " << casesResult.iterations.at(i).retValue;
//      }
//      os << std::endl;
//
//      size += step;
//    }
//  }
};

template <class TestSubject>
TestSubject copyDefault(const TestSubject &subject)
{
  return subject;
}

template <class TestSubject>
void deleteDefault(const TestSubject &subject){};

template <class TestSubject>
using CasesVec = std::vector<Case<TestSubject>>;

template <class TestSubject>
struct TestSubjectInfo
{
  explicit TestSubjectInfo(
      const std::function<TestSubject(int size)> &generator, unsigned subjectCount = 1,
      const std::function<TestSubject(const TestSubject &)> &copyFunc = copyDefault<TestSubject>,
      const std::function<void(const TestSubject &)> &delFunc = deleteDefault<TestSubject>)
      : generator(generator), subjectCount(subjectCount), copyFunc(copyFunc), delFunc(delFunc)
  {
  }

  std::function<TestSubject(int size)> generator;
  unsigned subjectCount;
  std::function<TestSubject(const TestSubject &)> copyFunc;
  std::function<void(const TestSubject &)> delFunc;
};

template <class TestSubject>
class SpeedTester
{
 private:
  Settings settings;
  TestSubjectInfo<TestSubject> subjectInfo;
  CasesVec<TestSubject> cases;

  std::vector<TestSubject> GenerateSubjects(int size)
  {
    std::vector<TestSubject> subjects;

    for (int i = 0; i < subjectInfo.subjectCount; i++)
    {
      subjects.push_back(subjectInfo.generator(size));
    }

    return subjects;
  }

  void DeleteSubjects(std::vector<TestSubject> &subjects)
  {
    for (TestSubject &subject : subjects)
    {
      subjectInfo.delFunc(subject);
    }
  }

  std::vector<TestSubject> CopySubjects(const std::vector<TestSubject> &subjects)
  {
    std::vector<TestSubject> newSubjects;

    for (const TestSubject &subject : subjects)
    {
      newSubjects.push_back(subjectInfo.copyFunc(subject));
    }

    return subjects;
  }

 public:
  SpeedTester(Settings settings, TestSubjectInfo<TestSubject> subjectInfo,
              CasesVec<TestSubject> tests)
      : settings(settings), subjectInfo(subjectInfo), cases(tests)
  {
  }

  Results PerformTests()
  {
    unsigned step = settings.CalcStep();
    std::vector<CaseResult> casesResults;

    for (int i = 0; i < cases.size(); i++)
    {
      casesResults.push_back(CaseResult(cases[i].caseName));
    }

    for (unsigned size = settings.minSize; size < settings.maxSize; size += step)
    {
      std::vector<TestSubject> subjects = GenerateSubjects(size);

      for (int i = 0; i < cases.size(); i++)
      {
        std::vector<TestSubject> subjectsClones = CopySubjects(subjects);
        IterationResult iterationResult;

        auto begin = steady_clock::now();
        cases[i].func(subjectsClones);
        auto end = steady_clock::now();

        iterationResult.elapsedTime = duration_cast<milliseconds>(end - begin);

        casesResults.at(i).iterations.push_back(iterationResult);
        DeleteSubjects(subjectsClones);
      }

      DeleteSubjects(subjects);
    }

    Results ret = {casesResults, settings};
    return ret;
  }

  Settings GetSettings() const { return settings; }
  TestSubjectInfo<TestSubject> GetSubjectInfo() const { return subjectInfo; }
  CasesVec<TestSubject> GetTests() const { return cases; }

  void SetSettings(const Settings &settings) { this->settings = settings; }
  void SetSubjectInfo(const TestSubjectInfo<TestSubject> &subjectInfo)
  {
    this->subjectInfo = subjectInfo;
  }
  void SetTests(const CasesVec<TestSubject> &tests) { this->cases = tests; }
};
}  // namespace SpeedTest