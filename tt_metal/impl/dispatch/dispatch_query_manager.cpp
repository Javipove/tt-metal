// SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "tt_metal/impl/dispatch/dispatch_query_manager.hpp"

namespace {

CoreType dispatch_core_type() {
    CoreType dispatch_core_type;
    CoreType first_core_type;
    for (chip_id_t device_id = 0; device_id < tt::Cluster::instance().number_of_devices(); device_id++) {
        dispatch_core_type = tt::tt_metal::dispatch_core_manager::instance().get_dispatch_core_type(device_id);
        if (device_id == 0) {
            first_core_type = dispatch_core_type;
        } else {
            TT_FATAL(
                dispatch_core_type == first_core_type,
                "Expected the Dispatch Core Type to be consistent across physical devices");
        }
    }
    return dispatch_core_type;
}

}  // namespace
namespace tt::tt_metal {

DispatchQueryManager* inst_ = nullptr;
void DispatchQueryManager::initialize(uint8_t num_hw_cqs) {
    if (inst_ == nullptr) {
        static DispatchQueryManager DispatchQueryManager(num_hw_cqs);
        inst_ = &DispatchQueryManager;
    } else if (num_hw_cqs != inst_->num_hw_cqs_ or dispatch_core_type() != inst_->dispatch_core_type_) {
        inst_->reset(num_hw_cqs);
        inst_->num_hw_cqs_ = num_hw_cqs;
    }
}

const DispatchQueryManager& DispatchQueryManager::instance() {
    TT_FATAL(inst_ != nullptr, "Trying to acess the dispatch query layer without initializing it.");
    return *inst_;
}

bool DispatchQueryManager::dispatch_s_enabled() const { return dispatch_s_enabled_; }

bool DispatchQueryManager::distributed_dispatcher() const { return distributed_dispatcher_; }

NOC DispatchQueryManager::go_signal_noc() const { return go_signal_noc_; }

void DispatchQueryManager::reset(uint8_t num_hw_cqs) {
    num_hw_cqs_ = num_hw_cqs;
    dispatch_core_type_ = dispatch_core_type();
    dispatch_s_enabled_ = (num_hw_cqs == 1 or dispatch_core_type_ == CoreType::WORKER);
    distributed_dispatcher_ = (num_hw_cqs == 1 and dispatch_core_type_ == CoreType::ETH);
    go_signal_noc_ = dispatch_s_enabled_ ? NOC::NOC_1 : NOC::NOC_0;
}

DispatchQueryManager::DispatchQueryManager(uint8_t num_hw_cqs) { this->reset(num_hw_cqs); }

}  // namespace tt::tt_metal
