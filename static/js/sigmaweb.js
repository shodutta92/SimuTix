for (i = 0; i < bind.data.length; i++) {    
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
          .datum(bind.data[i])
          .transition().duration(500)
          .call(chart);

      nv.utils.windowResize(chart.update);

      return chart;
    }
    nv.addGraph(callback(bind, i));
}