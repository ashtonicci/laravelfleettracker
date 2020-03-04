<?php

namespace App\Http\Controllers\Api\v0;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Log;

class ApiController extends Controller
{
    function post_location(Request $request) {
        $lat = $request->input('lat');
        $long = $request->input('long');
        $location = ["lat"=>$lat, "long"=>$long];
        Log::Info($location);
        return response()->json(['status'=>'success', 'data'=>$location]);
    }
}
