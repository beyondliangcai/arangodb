////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
////////////////////////////////////////////////////////////////////////////////

#ifndef REST_SERVER_SERVER_FEATURE_H
#define REST_SERVER_SERVER_FEATURE_H 1

#include "ApplicationFeatures/ApplicationFeature.h"

#include "Rest/OperationMode.h"

namespace arangodb {
namespace rest {
class HttpHandlerFactory;
class AsyncJobManager;
}

class ServerFeature final : public application_features::ApplicationFeature {
 public:
  ServerFeature(application_features::ApplicationServer* server, int*);

 public:
  void collectOptions(std::shared_ptr<options::ProgramOptions>) override final;
  void validateOptions(std::shared_ptr<options::ProgramOptions>) override final;
  void start() override final;
  void beginShutdown() override final;
  void stop() override final;

 public:
  OperationMode operationMode() const { return _operationMode; }

 private:
  bool _console;
  bool _restServer;
  std::vector<std::string> _unitTests;
  std::vector<std::string> _scripts;
  std::vector<std::string> _scriptParameters;

 private:
  void waitForHeartbeat();
  int runUnitTests();
  int runScript();

 private:
  int* _result;
  OperationMode _operationMode;
};
}

#endif
