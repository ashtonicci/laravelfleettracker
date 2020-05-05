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
