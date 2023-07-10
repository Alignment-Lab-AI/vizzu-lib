import { data } from "../../test_data/chart_types_eu.mjs";

/** Title should fade in beside the chart, not after. */


let eh = (e) => { e.preventDefault() };

const testSteps = [
  (chart) => {
    let data = {
      series: [
        { name: "Foo", values: ["Alice", "Bob", "Ted"] },
        { name: "Bar", values: [15, 32, 12] },
        { name: "Baz", values: [5, 3, 2] },
      ],
    };

    return chart.animate({ data });
  },
  (chart) => {
	chart.on('plot-axis-label-draw', eh);
	chart.on('plot-marker-label-draw', eh);
    return chart.animate({
      x: "Foo",
      y: "Bar",
    });
  },
  (chart) => {
	chart.off('plot-axis-label-draw', eh);
	chart.off('plot-marker-label-draw', eh);
    return chart.animate({
      x: "Foo",
      y: "Bar",
      geometry: 'circle',
    });
  },
];

export default testSteps;
