#ifndef CHANNEL_H
#define CHANNEL_H


class Channel
{
    public:
        Channel();

        /**
         * @brief configure Configures the current channel for making it
         * functional
         */
        virtual void configure() = 0;
};

#endif // CHANNEL_H
