#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mqtt/client.h>
#include <mqtt/connect_options.h>
#include <iostream>
#include <functional>

class sample_mem_persistence : virtual public mqtt::iclient_persistence
{
	// Whether the store is open
	bool open_;

	// Use an STL map to store shared persistence pointers
	// against string keys.
	std::map<std::string, std::string> store_;

public:
	sample_mem_persistence() : open_(false) {}

	// "Open" the store
	void open(const std::string& clientId, const std::string& serverURI) override {
		std::cout << "[Opening persistence store for '" << clientId
			<< "' at '" << serverURI << "']" << std::endl;
		open_ = true;
	}

	// Close the persistent store that was previously opened.
	void close() override {
		std::cout << "[Closing persistence store.]" << std::endl;
		open_ = false;
	}

	// Clears persistence, so that it no longer contains any persisted data.
	void clear() override {
		std::cout << "[Clearing persistence store.]" << std::endl;
		store_.clear();
	}

	// Returns whether or not data is persisted using the specified key.
	bool contains_key(const std::string &key) override {
		return store_.find(key) != store_.end();
	}

	// Returns the keys in this persistent data store.
	mqtt::string_collection keys() const override {
		mqtt::string_collection ks;
		for (const auto& k : store_)
			ks.push_back(k.first);
		return ks;
	}

	// Puts the specified data into the persistent store.
	void put(const std::string& key, const std::vector<mqtt::string_view>& bufs) override {
		std::cout << "[Persisting data with key '"
			<< key << "']" << std::endl;
		std::string str;
		for (const auto& b : bufs)
			str.append(b.data(), b.size());	// += b.str();
		store_[key] = std::move(str);
	}

	// Gets the specified data out of the persistent store.
	std::string get(const std::string& key) const override {
		std::cout << "[Searching persistence for key '"
			<< key << "']" << std::endl;
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		std::cout << "[Found persistence data for key '"
			<< key << "']" << std::endl;

		return p->second;
	}

	// Remove the data for the specified key.
	void remove(const std::string &key) override {
		std::cout << "[Persistence removing key '" << key << "']" << std::endl;
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		store_.erase(p);
		std::cout << "[Persistence key removed '" << key << "']" << std::endl;
	}
};

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
    sample_mem_persistence * m_persist;
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
        delete(m_persist);
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

};

#endif