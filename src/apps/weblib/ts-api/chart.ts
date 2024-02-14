import * as Anim from './types/anim.js'
import * as Config from './types/config.js'
import * as Styles from './types/styles.js'
import * as D from './types/data.js'
import { Module } from './module/module.js'
import { CChart, Snapshot } from './module/cchart.js'
import { CAnimControl, CAnimation } from './module/canimctrl.js'
import { CData } from './module/cdata.js'
import { Render } from './render.js'
import { Data } from './data.js'
import { Events, EventType, EventHandler, EventMap } from './events.js'
import { PluginRegistry, Hooks } from './plugins.js'
import { Logging } from './plugins/logging.js'
import { Shorthands } from './plugins/shorthands.js'
import { PivotData } from './plugins/pivotdata.js'
import { Tooltip } from './plugins/tooltip.js'
import { PointerEvents } from './plugins/pointerevents.js'
import { CSSProperties } from './plugins/cssproperties.js'
import { Mirrored } from './tsutils.js'
import { HtmlCanvas } from './htmlcanvas.js'
import { VizzuOptions } from './vizzu.js'
import { AnimControl } from './animcontrol.js'
import { CoordSystem } from './plugins/coordsys.js'

export class Chart {
	private _cChart: CChart
	private _render: Render
	private _module: Module
	private _canvas: HtmlCanvas
	private _cData: CData
	private _data: Data
	private _events: Events
	private _plugins: PluginRegistry
	private _updateInterval?: ReturnType<typeof setTimeout>

	constructor(module: Module, options: VizzuOptions, plugins: PluginRegistry) {
		this._plugins = plugins
		this._module = module

		this._canvas = new HtmlCanvas(HtmlCanvas.extractOptions(options))

		this._cChart = this._module.createChart()
		this._cData = this._module.getData(this._cChart)
		this._data = new Data(this._cData)

		this._render = new Render(this._module, this._cChart, this._canvas)
		this._events = new Events(this._cChart, this._canvas)
		this._plugins.init(this._events)
	}

	registerBuiltins(): void {
		this._plugins.register(new Logging(this._module.setLogging.bind(this._module)), false)
		this._plugins.register(this._canvas, true)
		this._plugins.register(this._render, true)
		this._plugins.register(new CoordSystem(this._module.getCoordSystem(this._cChart)), true)
		this._plugins.register(new CSSProperties(), false)
		this._plugins.register(new Shorthands(), true)
		this._plugins.register(new PivotData(), true)
		this._plugins.register(new PointerEvents(this._cChart), true)
		this._plugins.register(new Tooltip(), false)
	}

	start(): void {
		if (!this._updateInterval) {
			this._render.updateFrame()
			this._updateInterval = setInterval(() => {
				this._render.updateFrame()
			}, 25)
		}
	}

	async prepareAnimation(
		target: Anim.Keyframes | CAnimation,
		options?: Anim.ControlOptions & Anim.Options
	): Promise<void> {
		const ctx = Object.assign({ target }, options !== undefined ? { options } : {})
		await this._plugins.hook(Hooks.prepareAnimation, ctx).default((ctx) => {
			this.setAnimParams(ctx.target, ctx.options)
		})
	}

	runAnimation(callback: (ok: boolean) => void): void {
		const ctx = { callback }
		this._plugins.hook(Hooks.runAnimation, ctx).default((ctx) => {
			this._cChart.animate(ctx.callback)
		})
	}

	setAnimParams(
		target: Anim.Keyframes | CAnimation,
		options?: Anim.ControlOptions & Anim.Options
	): void {
		if (target instanceof CAnimation) {
			this._cChart.restoreAnim(target)
		} else {
			for (const keyframe of target) this._setKeyframe(keyframe.target, keyframe.options)
		}
		if (options) this._cChart.animOptions.set(options)
	}

	private _setKeyframe(target: Anim.Target | Snapshot, options?: Anim.Options): void {
		if (target) {
			if (target instanceof Snapshot) {
				this._cChart.restoreSnapshot(target)
			} else {
				if (target.data) this._data.set(target.data)
				if (target.style) this._cChart.style.set(target.style)
				if (target.config) {
					this._nullEmptyChannels(target.config)
					this._cChart.config.set(target.config)
				}
			}
		}
		if (options) this._cChart.animOptions.set(options)
		this._cChart.setKeyframe()
	}

	private _nullEmptyChannels(config: Config.Chart): void {
		if (!config.channels) return
		Object.values(config.channels).forEach((channel: Config.Channel): void => {
			if (Array.isArray(channel?.set) && channel?.set.length === 0) {
				channel.set = null
			}
		})
	}

	destruct(): void {
		this._canvas.destruct()
		if (this._updateInterval) clearInterval(this._updateInterval)
		delete this._updateInterval
	}

	version(): string {
		return this._module.version()
	}

	get data(): Mirrored<D.Metainfo> {
		return this._cData.getMetaInfo()
	}

	get config(): Mirrored<Config.Chart> {
		return this._cChart.config.get()
	}

	get style(): Mirrored<Styles.Chart> {
		return this._cChart.style.get()
	}

	getComputedStyle(): Mirrored<Styles.Chart> {
		return this._cChart.computedStyle.get()
	}

	on<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
		this._events.add(eventName, handler)
	}

	off<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
		this._events.remove(eventName, handler)
	}

	store(): Snapshot {
		return this._cChart.storeSnapshot()
	}

	getAnimControl(): AnimControl {
		return new AnimControl(this._getCAnimControl())
	}

	private _getCAnimControl(): CAnimControl {
		return this._module.getAnimControl(this._cChart)
	}
}
