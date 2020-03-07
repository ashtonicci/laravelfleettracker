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
        Log::info($request);
        // $location = new Location;
        // $location->lat = (double) $request->input('lat');
        // $location->long = (double) $request->input('long');
        // $location->save();
        // event(new NewLocation($location));
        return response()->json(['status'=>200]);
    }
}
