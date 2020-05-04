<template>
    <div class="container">
        <div class="row justify-content-center">
            <div class="col-md-8">
                <div id="mapid"></div>
            </div>
        </div>
    </div>
</template>

<script>
    export default {
        mounted() {
            var channel = Echo.channel('location');
            $( document ).ready(function() {
                var mymap = L.map('mapid').setView([53.557699, -2.869379], 16);
                L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
                    attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, <a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
                    id: 'mapbox/streets-v11',
                    tileSize: 512,
                    zoomOffset: -1,
                    accessToken: 'pk.eyJ1IjoiYXNodG9uaWNjaSIsImEiOiJjazdkaWtiaG0wdmZlM21tc3Vub3JsODk4In0.7vwJBP25-7XM4ofA9ijhIQ'
                }).addTo(mymap);
                var marker = L.marker([51.508, -3.0]).addTo(mymap);
                channel.listen('.new-location', function(data) {
                    var latlng = L.latLng(data.location.lat, data.location.long);
                    marker.setLatLng(latlng);
                });
            });
        }
    }
</script>
