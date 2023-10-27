import { CEnv, CObject } from './cenv.js'
import { CPointerClosure } from './objregistry.js'
import { CColorGradient } from './ccolorgradient.js'
import { CString, CColorGradientPtr } from '../cvizzu.types'

export class CCanvas extends CObject {
  constructor(env: CEnv, getId: CPointerClosure) {
    super(getId, env)
  }

  getColorGradient(stops: CColorGradientPtr, stopCount: number): CColorGradient {
    return new CColorGradient(this, stops, stopCount)
  }

  getString(text: CString): string {
    return this._wasm.UTF8ToString(text)
  }
}
