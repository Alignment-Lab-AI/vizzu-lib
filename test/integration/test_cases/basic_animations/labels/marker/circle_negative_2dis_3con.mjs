import { data } from '/test/integration/test_data/chart_types_eu.js';
data.filter = record =>
record.Country != 'Germany' &&
record.Country != 'Italy' &&
record.Country != 'Hungary' &&
record.Country != 'Netherlands' &&
record.Country != 'Estonia' &&
record.Country != 'Belgium' &&
record.Country != 'Sweden' &&
record.Country != 'Poland' &&
record.Country != 'Malta' &&
record.Country != 'Romania' &&
record.Country != 'Greece' &&
record.Country != 'Slovakia' &&
record.Country != 'Ireland' &&
record.Country != 'Lithuania' &&
record.Country != 'Croatia' &&
record.Country != 'Slovenia' &&
record.Country != 'Portugal' &&
record.Country != 'Finland' &&
record.Country != 'United Kingdom';

const testSteps = [
    chart => chart.animate(
        {   data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Circle Marker Label Positions - Center',
                geometry: 'circle',
            },
            style:
            {
                plot:
                { 
                    marker:
                    {
                        label:
                        {
                            position: 'center'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Top',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Left',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'left'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Bottom',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'bottom'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Right',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'right'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Horizontal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'center'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Tangential',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'tangential'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Normal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'normal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Vertical',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'vertical'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Top',
                coordSystem:'polar'
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Left',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'left'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Bottom',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'bottom'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Right',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'right'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Horizontal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'center',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Normal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'normal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Vertical',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'vertical'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Tangential',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'tangential'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Angle',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            angle: 0.5 * 3.14
                        }
                    }
                }
            }
        }
    ),        
    chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    y: { detach: ['Negative nums'], range: '0,1.1,%' },
                    x: { detach: ['Neg_huge'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Pos_small'] },
                    label: { attach: ['Pos_small'] }
                },
                title: 'Without Coordinate',
                coordSystem: 'cartesian'
            },
            style:
            {
                plot:
                {
                    marker:
                    {
                        label:
                        {
                            orientation: 'horizontal',
                            angle: null
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Top',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Left',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'left'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Bottom',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'bottom'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Right',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'right'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Horizontal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'center'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Tangential',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'tangential'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Normal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'normal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Circle Marker Label Positions - Center/Vertical',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'vertical'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;