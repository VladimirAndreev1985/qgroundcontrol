/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "VehicleGPS2FactGroup.h"
#include "Vehicle.h"
#include "QGCGeo.h"

#include <QtPositioning/QGeoCoordinate>

void VehicleGPS2FactGroup::handleMessage(Vehicle *vehicle, const mavlink_message_t &message)
{
    Q_UNUSED(vehicle);

    switch (message.msgid) {
    case MAVLINK_MSG_ID_GPS2_RAW:
        _handleGps2Raw(message);
        break;
    default:
        break;
    }
}

void VehicleGPS2FactGroup::_handleGps2Raw(const mavlink_message_t &message)
{
    mavlink_gps2_raw_t gps2Raw{};
    mavlink_msg_gps2_raw_decode(&message, &gps2Raw);

    lat()->setRawValue(gps2Raw.lat * 1e-7);
    lon()->setRawValue(gps2Raw.lon * 1e-7);
    mgrs()->setRawValue(QGCGeo::convertGeoToMGRS(QGeoCoordinate(gps2Raw.lat * 1e-7, gps2Raw.lon * 1e-7)));
    count()->setRawValue((gps2Raw.satellites_visible == 255) ? 0 : gps2Raw.satellites_visible);
    hdop()->setRawValue((gps2Raw.eph == UINT16_MAX) ? qQNaN() : (gps2Raw.eph / 100.0));
    vdop()->setRawValue((gps2Raw.epv == UINT16_MAX) ? qQNaN() : (gps2Raw.epv / 100.0));
    courseOverGround()->setRawValue((gps2Raw.cog == UINT16_MAX) ? qQNaN() : (gps2Raw.cog / 100.0));
    yaw()->setRawValue((gps2Raw.yaw == UINT16_MAX) ? qQNaN() : (gps2Raw.yaw / 100.0));
    lock()->setRawValue(gps2Raw.fix_type);

    _setTelemetryAvailable(true);
}
