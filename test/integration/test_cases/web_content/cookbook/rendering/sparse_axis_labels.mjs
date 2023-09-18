import { data } from '../../../../test_data/music_industry_history_1.mjs'

const testSteps = [
  (chart) => {
    chart.on('plot-axis-label-draw', (event) => {
      if (event.target.parent.id === 'x') {
        let year = parseFloat(event.target.value)
        if (year % 5 != 0) event.preventDefault()
      }
    })

    return chart.animate({
      data: data,
      config: {
        y: 'Revenue',
        x: { set: 'Year', ticks: true },
        title: 'Every 5th label shown on X'
      }
    })
  }
]

export default testSteps
