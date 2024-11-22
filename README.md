# Home Assistant JSON

Library for (de)serializing [Home Assistant MQTT](https://www.home-assistant.io/integrations/mqtt/) JSON format.

The library provides a set of classes that can be used to implement MQTT control support with Home Assistant without additional configuration. The classes are able to parse and serialize data, as well as automatically generate topic names.

## Usage

```cpp
#include <PubSubServer.h>
#include <HomeAssistantJSON/light_entity.h>
// Your internal platforms
#include <platforms/platforms.h>

HomeAssistant::Device device = HomeAssistant::Device({
    .name = "My Device",
    .id = "my_device",
    .mqttNamespace = "my/device"
});
HomeAssistant::LightEntity lightEntity({
    .name = "Light",
	.identifier = "light",
	.icon = "mdi:led-strip-variant",
    .effects = nullptr,
    .effectCount = 0,
	.writable = true,
	.maxMireds = 400,
	.minMireds = 120
}, device);

ILightPlatform* light;
HomeAssistant::LightState state;

void reportLightConfig(PubSubClient* client) {
	lightEntity.serializeConfig(responseBuffer);
	client->publish(lightEntity.configTopic(), responseBuffer);
}

void reportLightState(PubSubClient* client) {
	state.enabled = light->getPower();
	// ...
	lightEntity.serializeValue(responseBuffer, state);
	client->publish(lightEntity.stateTopic(), responseBuffer);
}

void updateLightState(PubSubClient* client, byte* payload, uint length) {
	lightEntity.parseValue(state, payload);
	// Update platform with new state
	light->setPower(state.enabled);
	// ...
}

void registerLightTopics(PubSubServer* server) {
	light = IO_INJECT(ILightPlatform);
	lightEntity.setEffects(light->effects(), light->effectCount());
	server
		->report(reportLightConfig, MQTT_CONFIG_REPORT_INTERVAL)
		->report(reportLightState, MQTT_STATE_REPORT_INTERVAL)
		->on(lightEntity.commandTopic(), updateLightState);
}
```
