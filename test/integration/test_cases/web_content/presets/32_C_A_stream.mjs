import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: chart.constructor.presets.stream({
                x: 'Year',
                y: 'Revenue [m$]',
                stackedBy: 'Format',
                title: 'Stream Graph'
            }),
            style: {
                plot: {
                    yAxis: {
                        interlacing: { color: '#ffffff00' }
                    },
                    xAxis: {
                        label: {
                            angle: '-45deg'
                        }
                    }
                }
            }
        })
];

export default testSteps;