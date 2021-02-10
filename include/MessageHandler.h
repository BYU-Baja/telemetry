template<class T>
class MessageHandler {
    public:
        template<typename U>
        virtual void onMessage(void(*handler)(T message));
};
