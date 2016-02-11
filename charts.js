var client = new Keen({
  projectId: "56bc6ee746f9a7051719100c",
  readKey: "52ce6fd8cc35a6b30af6659b4b1ca6db6defb6520d7f8ef55c7d505afd66c099bfba5c88cdaedaaac0142ae0740629e3d60ea95f63f1f3b1cd017d4004221eaf0ce98b3ee3f01dae126ba8d8f5326f3b447a644a76532bdad88318df4cf0b895"
});

var charts = {

  "chart-co2-2d": {interval: "hourly", targetProperty: "ppm", timeframe: "this_2_days"},
  "chart-co2-6h": {interval: "minutely", targetProperty: "ppm", timeframe: "this_6_hours"},

  "chart-t-2d":   {interval: "hourly", targetProperty: "t", timeframe: "this_2_days"},
  "chart-t-6h":   {interval: "minutely", targetProperty: "t", timeframe: "this_6_hours"},

  "chart-h-2d":   {interval: "hourly", targetProperty: "h", timeframe: "this_2_days"},
  "chart-h-6h":   {interval: "minutely", targetProperty: "h", timeframe: "this_6_hours"}

}

var titles = {

  "chart-co2-2d": "CO2, ppm (last 2 days)",
  "chart-co2-6h": "CO2, ppm (last 6 hours)",

  "chart-t-2d":   "Temp, C (last 2 days)",
  "chart-t-6h":   "Temp, C (last 6 hours)",

  "chart-h-2d":   "Humid, % (last 2 days)",
  "chart-h-6h":   "Humid, % (last 6 hours)"

}



var baseSettings = {
  eventCollection: "measurements",
  timezone: "UTC"
};

var chartsObjects = {}

Keen.ready(function(){
  _.each(charts, function(params, id){
    chartsObjects[id] = new Keen.Query("average", _.extend(baseSettings, params));
    var chart = client.draw(chartsObjects[id], document.getElementById(id), {
      // Custom configuration here

    });
    // setInterval(function() { req.refresh(); }, 1000 * 60 * 15);

    chart.attributes({ title: titles[id], height: 300 });
  });
});
