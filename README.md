# Laravel Fleet Tracker
This project is a fleet tracking system, it pairs an ESP2866 Micro-Controller with an UBlox NEO-6M GPS Module to form a GPS Receiver and posts its current position in latitude and longitude to a web application every 2 minutes, which then displays the position on a map in real time.

__GPS Receiver__
![GPS Receiver](https://i.imgur.com/jooK6Bx.jpg)

The GPS Receiver uses NeoGPS to parse raw NMEA strings into a more "human format" 

__Basic overview of the process__
![Overall Process Diagram](https://i.imgur.com/jGDDhQt.png)

__Screenshot of the location on the map__
![Location on map](https://i.imgur.com/QuyeDk4.png)

The project makes use of a Laravel MVC application with a Vue front end. Location is updated in real time using an event driven Pusher channel which receives the location object from the back end and sends it to the view via asynchronous listening function on the front end tied to the pusher channel. When new coordinates are received the map is updated.

__Example Location Object__

![Location Object](https://i.imgur.com/LU8PtUb.png)

As the location sent from the receiver uses high-precision coordinates, location is accurate to a matter of meters.

__Zoomed in Location__
![Zoomed in Location](https://i.imgur.com/pxpyJBu.png)

## Key Files
