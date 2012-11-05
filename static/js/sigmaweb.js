for (i = 0; i < bind.graphs.length; i++) {
    graphData = {}
    graphData['key'] = bind.graphs[i]['name']
    graphData['values'] = []
    for (j = 0; j < bind.data[bind.graphs[i]['x-axis']].length; j++) {
        graphData['values'].push({"x": bind.data[bind.graphs[i]['x-axis']][j],
                                     "y": bind.data[bind.graphs[i]['y-axis']][j]})
    
    }
    
    var callback = function(bind, i) {
      var chart = nv.models.lineWithFocusChart();
      
      chart.xAxis
          .tickFormat(d3.format(',f'))
          .axisLabel(bind.graphs[i]['x-display']);
      chart.x2Axis
          .tickFormat(d3.format(',f'))


      chart.yAxis
          .tickFormat(d3.format(',.2f'))
          .axisLabel(bind.graphs[i]['y-display']);
      chart.y2Axis
          .tickFormat(d3.format(',.2f'))


      d3.select('#charts #chart'+i)
          .datum([graphData,])
          .transition().duration(500)
          .call(chart);

      nv.utils.windowResize(chart.update);

      return chart;
    }
    nv.addGraph(callback(bind, i));
}