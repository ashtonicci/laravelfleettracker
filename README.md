# Laravel Fleet Tracker
This project is a fleet tracking system, it pairs an ESP2866 Micro-Controller with an UBlox NEO-6M GPS Module to form a GPS Receiver and posts its current position in latitude and longitude to a web application every 2 minutes, which then displays the position on a map in real time.

The project makes use of a Laravel MVC application with a Vue front end, the location is updated in real time every two minutes using an event driven Pusher channel. This receives the location object from the back end using an event. This location is then sent to the view via pusher channel which then sends the new coordinates to the map using an asynchronous listener function on the view.

As the location sent from the receiver uses high-precision coordinates, location is accurate to a matter of meters and as the GPS Receiver needs to be powered via battery, it makes use of ESP Deep Sleep commands between post requests, this keeps power consumption at a minimum with an idle consumption of 50mA and an consumption during post requests of 140mA. Minimal power saving measures have been applied and these figures can be reduced further using a greater level of optimisation. 

__Basic overview of the process__
![Overall Process Diagram](https://i.imgur.com/jGDDhQt.png)

## Key Components

__GPS Receiver__

![GPS Receiver](https://i.imgur.com/jooK6Bx.jpg)

The prototyped GPS Receiver

__Example Location Object__

![Location Object](https://i.imgur.com/LU8PtUb.png)

A created location object containing the lat and long sent from the GPS receiver

__Example location on the map__

![Location on map](https://i.imgur.com/pxpyJBu.png)

The map used is an open source Mapbox/Leaflet JS stack.

## Key Files

__ApiController.php__
                <?php

                namespace App\Http\Controllers\Api\v0;

                use App\Http\Controllers\Controller;
                use Illuminate\Http\Request;
                use App\Location;
                use Illuminate\Support\Facades\Log;
                use App\Events\NewLocation;

                class ApiController extends Controller
                {
                    function post_location(Request $request) {
                        $location = new Location;
                        $location->lat =  $request->input('lat') / 10000000;
                        $location->long = $request->input('long') / 10000000;
                        $location->save();
                        Log::info($location);
                        event(new NewLocation($location));
                        return response()->json(['status'=>200]);
                    }
                }

__MapComponent.vue__

                var channel = Echo.channel('location');
                $(document).ready(function () {
                    var mymap = L.map('mapid').setView([53.557699, -2.869379], 16);
                    L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
                        attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, <a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
                        id: 'mapbox/streets-v11',
                        tileSize: 512,
                        zoomOffset: -1,
                        accessToken: 'pk.eyJ1IjoiYXNodG9uaWNjaSIsImEiOiJjazdkaWtiaG0wdmZlM21tc3Vub3JsODk4In0.7vwJBP25-7XM4ofA9ijhIQ'
                    }).addTo(mymap);
                    var marker = L.marker([51.508, -3.0]).addTo(mymap);
                    channel.listen('.new-location', function (data) {
                        var latlng = L.latLng(data.location.lat, data.location.long);
                        marker.setLatLng(latlng);
                    });
                });

