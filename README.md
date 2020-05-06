# Laravel Fleet Tracker
This project is a fleet tracking system, it pairs an ESP2866 Micro-Controller with an UBlox NEO-6M GPS Module to form a GPS Receiver and posts its current position in latitude and longitude to a web application every 80 seconds, which then displays the position on a map in real time.

The project makes use of a [Laravel](https://laravel.com/) MVC application with a [VueJS](https://vuejs.org/) front end, the location is updated in real time every two minutes using an event driven Pusher channel. This receives the location object from the back end using an event. This location is then sent to the view via pusher channel which then sends the new coordinates to the map using an asynchronous listener function on the view.

As the location sent from the receiver uses high-precision coordinates, location is accurate to a matter of meters and as the GPS Receiver needs to be powered via battery, it makes use of ESP Deep Sleep commands between post requests, this keeps power consumption at a minimum with an idle consumption of 50mA and an consumption during post requests of 140mA. Minimal power saving measures have been applied and these figures can be reduced further using a greater level of optimisation. 

## Basic overview of the process 
![Overall Process Diagram](https://i.imgur.com/ZR034Ds.png)


## Key Components

### GPS Receiver

![GPS Receiver](https://i.imgur.com/jooK6Bx.jpg)

The prototyped GPS Receiver

### Example Location Object

![Location Object](https://i.imgur.com/LU8PtUb.png)

A created location object containing the lat and long sent from the GPS receiver

### Example location on the map

![Location on map](https://i.imgur.com/pxpyJBu.png)

The map used is an open source Mapbox/Leaflet JS stack.

## Key Files

### gps_receiver.ino - /sketch/gps_receiver 

Parses in raw NMEA strings and sends them in a post request to the location endpoint

                void loop() {
                  while (gps.available( gpsPort ))
                    doSomeWork( gps.read() );
                }

                static void doSomeWork( const gps_fix & fix )
                {
                  if (fix.valid.location) {
                    postData = "lat=" + String(fix.latitudeL()) + "&long=" + String(fix.longitudeL());
                    //IP Address of Web Server
                    http.begin("http://192.168.0.10:8000/api/v0/location/");
                    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                    int httpCode = http.POST(postData);   //Send the request
                    String payload = http.getString();    //Get the response payload  
                    DEBUG_PORT.println("code:");
                    DEBUG_PORT.println(httpCode);   //Print HTTP return code
                    DEBUG_PORT.println("data:");    //Print request response payload
                    DEBUG_PORT.println(payload);    //Print request response payload
                    http.end();  //Close connection
                  }
                  //Sleep for 60 seconds
                  ESP.deepSleep(60e6);
                }

### api.php - /routes/api.php 

Defines the route for the location API endpoint

                Route::group(['prefix' => 'v0'], function(){    
                    Route::post('/location', 'API\v0\ApiController@post_location'); 
                });

### ApiController.php - App/Http/Controllers/Api/V0 

Parses in the request object from the GPS receiver, creates the location object and fires the ```NewLocation``` event before returning a 200 OK Response.

                class ApiController extends Controller
                {
                    function post_location(Request $request) {
                        $location = new Location;
                        $location->lat =  $request->input('lat') / 10000000;
                        $location->long = $request->input('long') / 10000000;
                        $location->save();
                        event(new NewLocation($location));
                        return response()->json(['status'=>200]);
                    }
                }

__NewLocation.php - /App/Events/NewLocation.php__
Responsible for sending the location object to pusher, wrapping the location object in a ```new-location``` message and broadcasting it on the ```location``` channel

                class NewLocation implements ShouldBroadcast
                {
                    use Dispatchable, InteractsWithSockets, SerializesModels;

                    public $location; 

                    /**
                     * Create a new event instance.
                     *
                     * @return void
                     */

                    public function __construct(Location $location)
                    {
                        $this->location = $location;
                    }

                    /**
                     * Get the channels the event should broadcast on.
                     *
                     * @return \Illuminate\Broadcasting\Channel|array
                     */
                    public function broadcastOn()
                    {
                        return new Channel('location');
                    }

                    public function broadcastAs()
                    {
                        return 'new-location';
                    }
                }

### MapComponent.vue - /resources/js/components/MapComponent.vue 

Responsible for drawing the map and updating the location of the GPS receiver, the listener function listens for ```new-location``` messages on the ```location``` channel 

                var channel = Echo.channel('location');
                $(document).ready(function () {
                    var mymap = L.map('mapid').setView([53.557699, -2.869379], 16);
                    L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
                        attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, <a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
                        id: 'mapbox/streets-v11',
                        tileSize: 512,
                        zoomOffset: -1,
                        accessToken: '[REDACTED]'
                    }).addTo(mymap);
                    var marker = L.marker([51.508, -3.0]).addTo(mymap);
                    channel.listen('.new-location', function (data) {
                        var latlng = L.latLng(data.location.lat, data.location.long);
                        marker.setLatLng(latlng);
                    });
                });
                
## Build Instructions

### Web Application 
These are my build instructions for Linux
1. Clone this Repository
2. Install [VirtualBox 6.x](https://www.virtualbox.org/wiki/Downloads)
3. Install [Vagrant](https://www.vagrantup.com/downloads.html)
4. Install the Laravel Homestead Vagrant Box

                vagrant box add laravel/homestead
5. Clone Homestead Repository

                git clone https://github.com/laravel/homestead.git ~/Homestead
6. Run the ```bash init.sh``` commmand from the ```~/Homestead/``` directory
7. Set your VM provider in ```Homestead.yaml``` file

                provider: virtualbox
8. Map the project directory in ```Homestead.yaml```

                folders:
                    - map: ~/code/laravelfleettracker
                    to: /home/vagrant/code/laravelfleettracker
9. Map the site to the directory

                sites:
                    - map: ~/code/laravelfleettracker
                    to: /home/vagrant/code/laravelfleettracker/public 
9. Add IP and hostname to the ```etc/hosts``` file

                192.168.10.10  homestead.test
10. run ```vagrant up``` in the ```Homestead``` directory

11. run ```vagrant ssh``` in the ```Homestead``` directory to SSH to the vagrant box

12. change directory on the box to the project folder

                cd ~/code/laravelfleettracker

14. run ```php artisan migrate``` to run the necessary migrations and build the database

15. Navigate to ```http://homestead.test``` to see the running Web Application

16. run ```php artisan queue:listen```, locations are sent to pusher using an event, events are queue based and ```queue:listen``` runs the job in the queue as they arrive

### GPS Receiver

The hardware set up is as follows (With an added wiring running on the NodeMCU from RST to D0 to enable deep sleep functionality)

| NEO-6M GPS Module | NodeMCU |
| ------------- |:-------------:| 
| TX            | RX            |
| RX            | TX            |
| VCC           | 3V3           |
| GND           | GND           |

1. Open the sketch in Arduino IDE
2. Disconnect the cables from the D0 pin and the RX pin on the ESP2866 as these interfere with the uploading process
3. Upload the sketch

### Pusher
1. Sign up to pusher.com
2. In the setup screen, name the application and the select the EU cluster
3. Select Vue.JS as the front-end tech and Laravel as the back-end tech

![Pusher Demonstration](https://i.imgur.com/Xxa64W1.png)

4. Install pusher with composer (while ssh'd to the homestead box)
                
                composer require pusher/pusher-php-server
5. Add the ```app_id```, ```app_key``` and ```app_id``` for pusher in the .env file

6. Location objects should start to show in the debug console

![Location Object Messages](https://i.imgur.com/CFTQ1fP.png)
