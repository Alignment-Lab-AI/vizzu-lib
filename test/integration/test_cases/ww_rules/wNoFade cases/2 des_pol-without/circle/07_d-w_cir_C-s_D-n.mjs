import { data } from '../../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
                record.Country == 'Hungary'
        }),
        config:
        {
            channels:
            {
                y: { set: 'Value 3 (+)' },
                x: { set: 'Value 2 (+)' },
                color: { set: 'Country' },
                noop: { set: 'Year' }
            },
            title: 'Scatter plot',
            geometry: 'circle'
        }  
    }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                y: { set: 'Value 3 (+)', range: { min: '-100%', max: '200%' } },
                x: { set: 'Value 2 (+)', range: { min: '0%', max: '200%' }  },
                color: { set: null },
                noop: { set: null },
                size: { set: ['Year', 'Country'] }
            },
            title: 'Scatter plot (stacked).',
            align: 'center'
        }
    }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Scatter plot (stacked).'
        }
    },
    {
             duration: 0
     }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                y: { set: 'Value 3 (+)', range: { min: '-10%', max: '110%' } },
                x: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }  },
                color: { set: 'Joy factors' },
                lightness: { set: 'Value 2 (+)' },
                size: { set: null  }
            },
            title: 'Scatter plot'
        }
    }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },                
            noop: { set: 'Joy factors' },
            size: { set: 'Value 3 (+)' }
        },
        title: 'Bubble Chart'
    }
}
)
];

export default testSteps;