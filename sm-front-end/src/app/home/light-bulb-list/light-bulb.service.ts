import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { LightBulb } from 'src/app/models/light-bulb.model';

@Injectable({
  providedIn: 'root'
})
export class LightBulbService {
  private _lightBulbs = new Subject<LightBulb[]>();
  
  constructor() { }

  get lightBulbs(): Observable<LightBulb[]> {
    return this._lightBulbs.asObservable();
  }

  public fetchData() {
    let doors: LightBulb[] = [];

    doors.push(new LightBulb(0, "Luz Cocina", 0));
    doors.push(new LightBulb(0, "Luz Sala", 0));
    doors.push(new LightBulb(0, "Luz Baño", 1));
    doors.push(new LightBulb(0, "Luz Cuarto 1", 0));
    doors.push(new LightBulb(0, "Luz Cuarto 2", 0));

    this._lightBulbs.next(doors);
  }

  public changeState(id: number) {
    
  }
}
