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
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        } 
    }),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },
            noop: { set: 'Year' },
            size: { set: ['Year', 'Value 2 (+)'] },
            color: { set: 'Joy factors' }
        },
        title: 'Add new Disc Color & Change Geoms & CoordSys',
        geometry: 'circle'
    } 
}
)

];

export default testSteps;