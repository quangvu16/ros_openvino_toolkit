/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief a header file with declaration of Pipeline Manager class
 * @file pipeline_manager.hpp
 */
#ifndef DYNAMIC_VINO_LIB__PIPELINE_MANAGER_HPP_
#define DYNAMIC_VINO_LIB__PIPELINE_MANAGER_HPP_

#include <atomic>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>

#include <vino_param_lib/param_manager.h>
#include "dynamic_vino_lib/pipeline.h"
#include "dynamic_vino_lib/engines/engine_manager.h"

/**
 * @class PipelineManager
 * @brief This class manages the lifecycles of pipelines.
 */
class PipelineManager
{
public:
  /**
  * @brief Get the singleton instance of PipelineManager class.
  * The instance will be created when first call.
  * @return The reference of PipelineManager instance.
  */
  static PipelineManager& getInstance()
  {
    static PipelineManager manager_;
    return manager_;
  };

  std::shared_ptr<Pipeline> createPipeline(const Params::ParamManager::PipelineRawData& params);
  void removePipeline(const std::string& name);
  PipelineManager& updatePipeline(const std::string& name, const Params::ParamManager::PipelineRawData& params);

  void runAll();
  void stopAll();
  void joinAll();

  enum PipelineState
  {
    PipelineState_ThreadNotCreated = 0,
    PipelineState_ThreadStopped = 1,
    PipelineState_ThreadRunning = 2,
    PipelineState_ThreadPasued = 3,
    PipelineState_Error = 4
  };

  struct PipelineData
  {
    Params::ParamManager::PipelineRawData params;
    std::shared_ptr<Pipeline> pipeline;
    std::vector<std::shared_ptr<ros::NodeHandle>> spin_nodes;
    std::shared_ptr<std::thread> thread;
    PipelineState state;
  };
  void runService();
  std::map<std::string, PipelineData> getPipelines()
  {
    return pipelines_;
  }
  std::map<std::string, PipelineData>* getPipelinesPtr()
  {
    return &pipelines_;
  }

private:
  PipelineManager(){};
  PipelineManager(PipelineManager const&);
  void operator=(PipelineManager const&);
  void threadPipeline(const char* name);
  std::map<std::string, std::shared_ptr<Input::BaseInputDevice>> parseInputDevice(const PipelineData& params);
  std::map<std::string, std::shared_ptr<Outputs::BaseOutput>> parseOutput(const PipelineData& pdata);
  std::map<std::string, std::shared_ptr<dynamic_vino_lib::BaseInference>>
  parseInference(const Params::ParamManager::PipelineRawData& params);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createFaceDetection(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createAgeGenderRecognition(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createEmotionRecognition(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createHeadPoseEstimation(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createObjectDetection(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createObjectSegmentation(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createPersonReidentification(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createFaceReidentification(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createPersonAttribsDetection(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createVehicleAttribsDetection(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createLicensePlateDetection(const Params::ParamManager::InferenceRawData& infer);
  std::shared_ptr<dynamic_vino_lib::BaseInference>
  createLandmarksDetection(const Params::ParamManager::InferenceRawData& infer);

  std::map<std::string, PipelineData> pipelines_;
  Engines::EngineManager engine_manager_;
};

#endif  // DYNAMIC_VINO_LIB__PIPELINE_MANAGER_HPP_
