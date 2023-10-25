import { data } from '../../../../test_data/music_industry_history_1.mjs'
import { lastAnim } from '../../../../utils/utils.mjs'

const testSteps = [
  (chart) => {
    const input = document.createElement('input')
    input.id = 'slider'
    input.type = 'range'
    input.min = 1973
    input.max = 2020
    input.value = 2020
    input.style = 'width: 500px; position: absolute; top: 540px; left: 30px;'
    chart.getCanvasElement().parentElement.appendChild(input)

    input.oninput = (e) => {
      chart.animate(
        {
          data: { filter: (record) => record.Year === e.target.value }
        },
        { duration: '200ms' }
      )
    }

    data.filter = (record) => record.Year === '2020'

    return chart.animate({
      data,
      config: {
        x: 'Revenue',
        y: 'Format',
        color: 'Format',
        label: 'Revenue',
        sort: 'byValue',
        title: 'Filter with slider'
      }
    })
  },
  (chart) => {
    const input = document.getElementById('slider')
    input.value = '2000'
    const event = new Event('input')
    input.dispatchEvent(event)
    return lastAnim(chart)
  }
]

export default testSteps
