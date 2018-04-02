
class SerialCommunication {
public:
    SerialCommunication(int tx_pin, int rx_pin);

    virtual void updateBuffer();

    virtual bool isTheBufferAllRead();

    virtual char* getReadBuffer();

private:
    static const int MAX_BUFFER_LENGTH = 500;
    int tx_pin;
    int rx_pin;
    int buffer_index;
    char read_buffer[MAX_BUFFER_LENGTH+1];
};