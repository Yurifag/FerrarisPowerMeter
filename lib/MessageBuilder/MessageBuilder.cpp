#include <MessageBuilder.h>

MessageBuilder::MessageBuilder() {
}

void MessageBuilder::buildConfigrationPayload(const char* node, 
  const char* device_class,
  const char* state_class, 
  const char* name, 
  const char* unit_of_measurement, 
  const char* value_template,
  char* payload,
  uint16_t buf_size,
  uint8_t tNSize) {

  char stateTopicName[tNSize];   
  getStateTopicName(node, stateTopicName, tNSize);

  JSONVar jpayload;
  jpayload["device_class"] = device_class;
  jpayload["state_class"] = state_class;
  jpayload["name"] = name;
  jpayload["state_topic"] = stateTopicName;
  jpayload["json_attributes_topic"] = stateTopicName;
  jpayload["unique_id"] = String(name + String("-ESP"));
  jpayload["unit_of_measurement"] = unit_of_measurement;
  jpayload["value_template"] = value_template;
  
  strcpy(payload, JSONVar::stringify(jpayload).c_str());
}

void MessageBuilder::getStatePayload(const char* node, float powerInWatts, 
      float total_energy_in_kwh, float today_energy_in_kwh, char* buf, uint8_t buf_size) {
  
  JSONVar statePayload;
  statePayload["node"] = node;
  statePayload["total_energy_kwh"] = total_energy_in_kwh;
  statePayload["today_energy_kwh"] = today_energy_in_kwh;
  statePayload["power"] = powerInWatts;
  
  strcpy(buf, JSONVar::stringify(statePayload).c_str());
} 

void MessageBuilder::getConfigTopicName(const char* node, const char* device_class, char* tName, uint8_t buff_size) {
  strcpy(tName,"homeassistant/sensor/");
  strcat(tName, node);
  strcat(tName, "/");
  strcat(tName, device_class);
  strcat(tName, "/config");
  
}

void MessageBuilder::getStateTopicName(const char* node, char* tName, uint8_t buff_size) {
  strcpy(tName,"homeassistant/sensor/");
  strcat(tName, node);
  strcat(tName, "/state");
}

void MessageBuilder::getPowerConfigurationPayload(const char* node, char* payload, uint16_t buff_size, uint8_t tNSize) {
  char name[64];
  strcpy(name, node);
  strcat(name, "-Power");

  buildConfigrationPayload(node, "power", "measurement", name, "W", "{{value_json.power}}", payload, buff_size, tNSize);
}

void MessageBuilder::getEnergyConfigurationPayload(const char* node, char* payload, uint16_t buff_size, uint8_t tNSize) {
  char name[64];
  strcpy(name, node);
  strcat(name, "-Energy");

  buildConfigrationPayload(node, "energy", "total_increasing", name, "kWh", "{{value_json.total_energy_kwh}}", payload, buff_size, tNSize);
}
