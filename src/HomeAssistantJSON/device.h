#pragma once

#include <stdint.h>
#include "json_document.h"

namespace HomeAssistant {
    struct DeviceConfig {
		const char* name;
		const char* id;
		const char* mqttNamespace;
	};

	class Device {
	public:
		Device(
			DeviceConfig config
		) : config_(config) {}

		void fillConfig() {
			auto deviceNode = json["device"].to<JsonObject>();
			auto identifiers = deviceNode["identifiers"].to<JsonArray>();
			deviceNode["name"] = config_.name;
			identifiers.add(config_.id);
		}

        const char* name() const {
            return config_.name;
        }

        const char* id() const {
            return config_.id;
        }

        const char* mqttNamespace() const {
            return config_.mqttNamespace;
        }

    private:
        const DeviceConfig config_;
	};
};
