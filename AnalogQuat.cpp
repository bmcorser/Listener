#include <Urho3D/Input/InputEvents.h>

Quaternion ModifyQuat(Quaternion quat, StringHash eventType, VariantMap& eventData)
{
    using namespace JoystickAxisMove;

    float axis_pos = eventData[P_POSITION].GetFloat();
    Quaternion modified_quat;
    if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTX) {
        return Quaternion(quat.w_, quat.x_, -pos, quat.z_);
    }
    if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTY) {
        return Quaternion(quat.w_, -pos, quat.y_, quat.z_);
    }
}
