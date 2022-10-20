#include "MQTTClient.h"

void MQTTClient::Connect(const std::string& Client_ID){
    m_clientID = Client_ID;
    m_conOpt->set_keep_alive_interval(20);
    m_conOpt->set_clean_session(true);
    m_callback = new user_callback();

    m_client = new mqtt::client(m_url, Client_ID);
    
    m_client->set_callback(*m_callback);

    try{
        m_client->connect(*m_conOpt);
        m_isConnected = true;
    }catch(const mqtt::exception& exc){
        std::cout<<exc<<std::endl;
    }
}

void MQTTClient::SubscribeToTopic(const std::string& topic, int qos){
    std::cout<<"Subscribing..."<<std::endl;
    m_client->subscribe(topic, qos);
    std::cout<<"Ok"<<std::endl;
}
            
void MQTTClient::PublishToTopic(const std::string& message, const std::string& topic){
    try{
        mqtt::message msg(topic, message, 0, false);
        m_client->publish(msg);
    }catch(const mqtt::exception& exc){
        std::cout<<exc<<std::endl;
    }
 
}

void MQTTClient::StartListening(const std::function<void(const std::string&)>& callback){
     while(m_isConnected){
         auto msg = m_client->consume_message();

         if(msg){
             std::string payload = msg->to_string();
             callback(payload);
         }
     }
}