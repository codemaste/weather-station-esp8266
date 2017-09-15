
String HTTPgenIndexPage () {
  String s = "<!DOCTYPE HTML>";
  s += "<html>";
  s += "<head>";
  s += "<meta http-equiv='refresh' content='15'> ";
  s += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
  s += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css'>";
  s += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  s += "</head>";
  s += "<body>";

  s += "<div class='container'>";

  s += "<div class='page-header'>";
  s += "<h1>Climat <small>control</small></h1>";
  s += "</div>";
  
  s += "<div class='row'>";

  s += "<div class='col-xs-12 col-sm-12 col-md-6'>";
  s += "<div class='panel panel-info'>";
  s += "<div class='panel-heading'>Fan Mode</div>";
  s += "<div class='panel-body'>";
  if (vent_mode == -1) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-success' href='vent_auto'>auto</a> ";      
  if (vent_mode == 0) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-danger' href='vent_0'>off</a> ";     
  if (vent_mode == 1) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-info' href='vent_1'>1</a> ";   
  if (vent_mode == 2) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-info' href='vent_2'>2</a> ";   
  if (vent_mode == 3) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-info' href='vent_3'>3</a> ";   
  if (vent_mode == 4) s+= "<i class='glyphicon glyphicon-play'></i>"; s += "<a class='btn btn-lg btn-info' href='vent_4'>4</a> ";  
  s += "</div>";
  s += "</div>";
  s += "</div>";

  s += "<div class='col-xs-12 col-sm-12 col-md-6'>";
  s += "<div class='panel panel-default'>";
  s += "<div class='panel-heading'>Time</div>";
  s += "<div class='panel-body'>";
  int m = millis ();
//  s += "Now: " + returnDateTime (Rtc.GetDateTime ());    
//  s += "<br/>";
  s += "Uptime: " + String((int)(m/1000/60/60)) 
        + ":" +  String(((int)(m/1000/60)) % 60) 
        + ":" + String(((int)(m/1000)) % 60)
        + "." + String((int)(m%1000));
  s += "<br/>";
  s += "Narodmon sent: " + String (millis() - whNarodmonSent) + " ms ago";
  s += "<br/>";
  s += "</div>";
  s += "</div>";
  s += "</div>";

  s += "</div>";
  s += "<div class='row'>";

  s += "<div class='col-xs-12 col-sm-12 col-md-4'>";
  s += "<div class='panel panel-primary'>";
  s += "<div class='panel-heading'>CO<sub>2</sub></div>";
  s += "<div class='panel-body'>";
  s += "<h2>" + String(co2) + " ppm</h2>";
  s += "(for auto: " + String(recomend_vent) + ")";
  s += "</div>";
  s += "</div>";
  s += "</div>";

  s += "<div class='col-xs-6 col-sm-6 col-md-2'>";
  s += "<div class='panel panel-default'>";
  s += "<div class='panel-heading'>MQ</div>";
  s += "<div class='panel-body'>";
  s += "DUST ";
  if (dust >= protect_dust) s += "<span class='label label-danger'>";
  s += String(dust); 
  if (dust >= protect_dust) s += "</span>";
  s += "<br />";  
  s += "<form action='protect_set' method='post'>";
  s += "<input type='hidden' name='n' value='1' />";
  s += "<input type='text' name='v' size='4' value='" + String(protect_dust) + "' />";
  s += "<input type='submit' value='set' />";
  s += "</form>";
  s += "<br />";  
  s += "MQ4 ";
  if (mq4 >= protect_mq4) s += "<span class='label label-danger'>";
  s += String(mq4); 
  if (mq4 >= protect_mq4) s += "</span>";
  s += "<br />";  
  s += "<form action='protect_set' method='post'>";
  s += "<input type='hidden' name='n' value='4' />";
  s += "<input type='text' name='v' size='4' value='" + String(protect_mq4) + "' />";
  s += "<input type='submit' value='set' />";
  s += "</form>";
  s += "<br />";  
  s += "MQ136 ";
  if (mq136 >= protect_mq136) s += "<span class='label label-danger'>";
  s += String(mq136); 
  if (mq136 >= protect_mq136) s += "</span>";
  s += "<br />";  
  s += "<form action='protect_set' method='post'>";
  s += "<input type='hidden' name='n' value='136' />";
  s += "<input type='text' name='v' size='4' value='" + String(protect_mq136) + "' />";
  s += "<input type='submit' value='set' />";
  s += "</form>";
  s += "<br />";  
  s += "MQ137 ";
  if (mq137 >= protect_mq137) s += "<span class='label label-danger'>";
  s += String(mq137); 
  if (mq137 >= protect_mq137) s += "</span>";
  s += "<br />";  
  s += "<form action='protect_set' method='post'>";
  s += "<input type='hidden' name='n' value='137' />";
  s += "<input type='text' name='v' size='4' value='" + String(protect_mq137) + "' />";
  s += "<input type='submit' value='set' />";
  s += "</form>";
  
  s += "</div>";
  s += "</div>";
  s += "</div>";
  
  s += "<div class='col-xs-6 col-sm-6 col-md-2'>";
  s += "<div class='panel panel-default'>";
  s += "<div class='panel-heading'>DHT22</div>";
  s += "<div class='panel-body'>";
  s += "" + String(dht_t) + "&deg;C<br/>";
  s += "" + String(dht_h) + "%";
  s += "</div>";
  s += "</div>";
  

  s += "<div class='panel panel-default'>";
  s += "<div class='panel-heading'>HTU21</div>";
  s += "<div class='panel-body'>";
  s += "" + String(htu_t) + "&deg;C<br/>";
  s += "" + String(htu_h) + "%";
  s += "</div>";
  s += "</div>";
  
  s += "</div>";
  
  s += "<div class='col-xs-6 col-sm-6 col-md-2'>";
  s += "<div class='panel panel-success'>";
  s += "<div class='panel-heading'>AM2315</div>";
  s += "<div class='panel-body'>";
  s += "" + String(am_t) + "&deg;C<br/>";
  s += "" + String(am_h) + "%";
  s += "</div>";
  s += "</div>";
  s += "</div>";
  
  s += "<div class='col-xs-6 col-sm-6 col-md-2'>";
  s += "<div class='panel panel-info'>";
  s += "<div class='panel-heading'>BMP-180</div>";
  s += "<div class='panel-body'>";
  s += "" + String(bmp_t) + "&deg;C<br/>";
  s += "" + String(bmp_p) + "mm";
  s += "</div>";
  s += "</div>";
  s += "</div>";

  s += "</div>"; // row

  
  s += "</div>"; // container
  
  s += "</body>";
  s += "</html>";
  return s;
}

String HTTPgenXML () {
  String s = "";
  s += "<fan_mode>" + String(vent_mode) + "</fan_mode>" + "\n";
  s += "<fan_recommend>" + String(recomend_vent) + "</fan_recommend>" + "\n";
  s += "<fan_current>" + String(current_vent) + "</fan_current>" + "\n";
  s += "<uptime>" + String(millis()) + "</uptime>" + "\n";

  s += "<co2>" + String(co2) + "</co2>" + "\n";
  s += "<dust>" + String(dust) + "</dust>" + "\n";
  s += "<mq4>" + String(mq4) + "</mq4>" + "\n";
  s += "<mq136>" + String(mq136) + "</mq136>" + "\n";
  s += "<mq137>" + String(mq137) + "</mq137>" + "\n";
  s += "<dht_t>" + String(dht_t) + "</dht_t>" + "\n";
  s += "<dht_h>" + String(dht_h) + "</dht_h>" + "\n";
  s += "<htu_t>" + String(htu_t) + "</htu_t>" + "\n";
  s += "<htu_h>" + String(htu_h) + "</htu_h>" + "\n";
  s += "<am_t>" + String(am_t) + "</am_t>" + "\n";
  s += "<am_h>" + String(am_h) + "</am_h>" + "\n";
  s += "<bmp_t>" + String(bmp_t) + "</bmp_t>" + "\n";
  s += "<bmp_p>" + String(bmp_p) + "</bmp_p>" + "\n";

  return s;
}

