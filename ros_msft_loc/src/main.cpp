#pragma comment(lib, "windowsapp")

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 1 // The C++ Standard doesn't provide equivalent non-deprecated functionality yet.

#include <ros/ros.h>
#include <sensor_msgs/NavSatStatus.h>
#include <sensor_msgs/NavSatFix.h>
#include <vcruntime.h>
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Geolocation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Devices::Geolocation;

int main(int argc, char **argv)
{
    const int kDefaultFrequency = 2000;
    int frequency;
    std::string linkName;
    std::string prefix;
    winrt::init_apartment();
    ros::init(argc, argv, "ros_msft_loc");

    ros::NodeHandle nh;
    ros::NodeHandle nhPrivate("~");

    if (!nhPrivate.getParam("frequency", frequency))
    {
        frequency = kDefaultFrequency;
    }

    if (!nhPrivate.getParam("frame_id", linkName))
    {
        linkName = "gps";
    }

    if (!nhPrivate.searchParam("tf_prefix", prefix))
    {
        linkName = prefix + "/" + linkName;
    }

    ros::Publisher fix_pub = nh.advertise<sensor_msgs::NavSatFix>("fix", 1);
    //ros::Publisher vel_pub = nh.advertise<geometry_msgs::TwistStamped>("fix", 1);

    Geolocator geolocator;
    auto accessStatus = Geolocator::RequestAccessAsync().get();

    switch (accessStatus)
    {
        case GeolocationAccessStatus::Allowed:
        {
            geolocator.ReportInterval(frequency);
            geolocator.PositionChanged([&](Geolocator, PositionChangedEventArgs args)
            {
                auto coordinate = args.Position().Coordinate();

                sensor_msgs::NavSatFix navSatFix;
                navSatFix.header.frame_id = linkName;
                navSatFix.header.stamp = ros::Time();
                navSatFix.status.status = sensor_msgs::NavSatStatus::STATUS_SBAS_FIX;
                navSatFix.status.service = sensor_msgs::NavSatStatus::SERVICE_GPS;
                navSatFix.latitude = coordinate.Latitude();
                navSatFix.longitude = coordinate.Longitude();
                navSatFix.altitude = coordinate.Altitude().GetDouble();
                //navSatFix.position_covariance = sensor_msgs::NavSatFix::COVARIANCE_TYPE_UNKNOWN;
                fix_pub.publish(navSatFix);
            });
        }
        break;

        case GeolocationAccessStatus::Denied:
        {
            ROS_WARN("Geolocation: The user or system denied access to geolocation services");
        }
        break;

        default:
        {
            ROS_ERROR("Geolocation: Unable to access geolocation services");
        }
        break;
    }

    ros::spin();
}