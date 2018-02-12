////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
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
/// @author Kaveh Vahedipour
/// @author Matthew Von-Maszewski
////////////////////////////////////////////////////////////////////////////////

#include "CreateCollection.h"

#include "ApplicationFeatures/ApplicationServer.h"
#include "Basics/VelocyPackHelper.h"
#include "RestServer/CollectionFeature.h"
#include "VocBase/Methods/Collections.h"
#include "VocBase/Methods/Databases.h"

using namespace arangodb::application_features;
using namespace arangodb::maintenance;
using namespace arangodb::methods;

CreateCollection::CreateCollection(ActionDescription const& d) :
  ActionBase(d, arangodb::maintenance::FOREGROUND) {
  TRI_ASSERT(d.has(COLLECTION));
  TRI_ASSERT(d.has(DATABASE));
}

CreateCollection::~CreateCollection() {};

arangodb::Result CreateCollection::run(
  std::chrono::duration<double> const&, bool& finished) {
  arangodb::Result res;

  auto const& database = _description.get(DATABASE);
  auto const& collection = _description.get(COLLECTION);

  auto vocbase = Databases::lookup();
  if (vocbase == nullptr) {
    std::string errorMsg("CreateCollection: Failed to lookup database ");
    errorMsg += database;
    return actionError(ERROR_ARANGO_DATABASE_NOT_FOUND, errorMsg);
  }

  res = Collections::create(
    vocbase, collection, TRI_COL_TYPE_DOCUMENT, _description.properties(),
    true, true);
  
  return res;
}

arangodb::Result CreateCollection::kill(Signal const& signal) {
  return actionError(
    ERROR_ACTION_OPERATION_UNABORTABLE, "Cannot kill CreateCollection action");
}

arangodb::Result CreateCollection::progress(double& progress) {
  progress = 0.5;
  return arangodb::Result(ERROR_NO_ERROR);
}


