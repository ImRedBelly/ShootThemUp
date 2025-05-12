#pragma once

class AnimUtils
{
public:
    template <typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (IsValid(Animation))
        {

            const auto NotifyEvents = Animation->Notifies;
            for (auto NotifyEvent : NotifyEvents)
            {
                if (auto AnimNotify = Cast<T>(NotifyEvent.Notify))
                {
                    return AnimNotify;
                }
            }
        }
        return nullptr;
    }
};