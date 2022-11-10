#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mqtt/client.h>
#include <mqtt/connect_options.h>
#include <iostream>
#include <functional>

class user_callback : public virtual mqtt::callback
{
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		std::cout << "\n\t[Delivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << "]" << std::endl;
	}

public:
};

class MQTTClient{

    private: 
    std::string m_port;
    std::string m_host;
    std::string m_protocol;

    std::string m_user;
    std::string m_passwrd;

    std::string m_url;

	std::string m_clientID;

    mqtt::connect_options * m_conOpt;
    mqtt::client * m_client;
	user_callback * m_callback;

	bool m_isConnected;

    public:
    //constructors
    /**
    *@param host: IP direction or domain name of teh server
    *@param port: port to be connected
    *@param protocol: wss, ws, tcp, ssl, tcl?
    */
    MQTTClient(const std::string& host, const std::string& port, const std::string& protocol):m_host(host), 
    m_port(port), m_protocol(protocol){
        m_url = m_protocol + "://" + m_host + ":" + m_port;
		m_isConnected = false;
        m_conOpt = new mqtt::connect_options();
    }

    MQTTClient(const std::string& host, const std::string& port, const std::string& protocol, const std::string& user, const std::string password):
    m_host(host), m_port(port), m_protocol(protocol), m_user(user), m_passwrd(password){
        m_url = m_protocol + "://" + m_host + ":" + m_port;
		m_isConnected = false;
        m_conOpt = new mqtt::connect_options(user, password);
    }


    ~MQTTClient(){
		m_isConnected = false;
        delete(m_conOpt);
        delete(m_client);
		delete(m_callback);
    }

	/**
	*@brief Connect to server
	*@param Client_ID id for this device in the broker
	*/
    void Connect(const std::string& Client_ID);

	/**
	*@brief subscribe to a topic
	*@param topic MQTT topic to be subscribed
	*/
    void SubscribeToTopic(const std::string& topic, int qos = 0);

	/**
	*@brief send a message to a topic
	*@param message message to be sent
	*@param topic send a message to the topic
	*/
    void PublishToTopic(const std::string& message, const std::string& topic);

	/**
	*@brief starts a listening to subscribed topics
	*@param callback function executed with string parameter with the message received
	*/
	void StartListening(const std::function<void(const std::string&)>& callback);
	void StartListening(const std::function<void(const char *)>& callbak);

};

#endif