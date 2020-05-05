# Laravel Fleet Tracker
This project is a fleet tracking system, it pairs an ESP2866 Micro-Controller with an UBlox NEO-6M GPS Module to form a GPS Receiver and posts its current position in latitude and longitude to a web application every 2 minutes, which then displays the position on a map in real time.

The project makes use of a Laravel MVC application with a Vue front end, the location is updated in real time every two minutes using an event driven Pusher channel. This receives the location object from the back end using an event. This location is then sent to the view via pusher channel which then sends the new coordinates to the map using an asynchronous listener function on the view.

As the location sent from the receiver uses high-precision coordinates, location is accurate to a matter of meters and as the GPS Receiver needs to be powered via battery, it makes use of ESP Deep Sleep commands between post requests, this keeps power consumption at a minimum with an idle consumption of 50mA and an consumption during post requests of 140mA. Minimal power saving measures have been applied and these figures can be reduced further using a greater level of optimisation. 

__Basic overview of the process__
![Overall Process Diagram](https://i.imgur.com/jGDDhQt.png)

## Key Components

__GPS Receiver__

![GPS Receiver](https://i.imgur.com/jooK6Bx.jpg)

__Example Location Object__

![Location Object](https://i.imgur.com/LU8PtUb.png)

__Example location on the map__

![Location on map](https://i.imgur.com/pxpyJBu.png)


## Key Files

