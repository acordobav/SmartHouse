import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { LightBulb } from 'src/app/models/light-bulb.model';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class LightBulbService {
  private _lightBulbs = new Subject<LightBulb[]>();
  private bulbList: LightBulb[] = [];

  constructor(private http: HttpClient) { 
    this.http.get<LightBulb[]>(environment.restapiUrl + '/light').subscribe(
      (bulbList) => {
        this.bulbList = bulbList;
        this.update();
      }
    );

  }

  get lightBulbs(): Observable<LightBulb[]> {
    return this._lightBulbs.asObservable();
  }

  public update() {
    this._lightBulbs.next(this.bulbList);
  }

  public changeState(id: number) {
    if(this.bulbList[id].state) {
      this.bulbList[id].state = 0;
    } else {
      this.bulbList[id].state = 1;
    }

    this.changeStateRequest(id);
    this.update();
  }

  /**
   * Method to request the change in the state in a bulb
   * @param id Bulb id
   */
  private changeStateRequest(id: number) {
    this.http.put(environment.restapiUrl + '/light/' + id.toString(), null).subscribe();
  }

  /**
   * Method to turn off all the bulbs
   */
  public turnOff() {
    this.changeAllStateRequest(0);

    for (let i = 0; i < this.bulbList.length; i++) {
      this.bulbList[i].state = 0;
    }
    this.update();
  }
  
  /**
   * Method to turn on all the bulbs
   */
  public turnOn() {
    this.changeAllStateRequest(1);

    for (let i = 0; i < this.bulbList.length; i++) {
      this.bulbList[i].state = 1;
    }
    this.update();
  }

  /**
   * Method to request the change in the state of
   * all the bulbs
   * @param state: 1 -> turn on, 0 -> turn off
   */
  private changeAllStateRequest(state: number) {
    this.http.put(environment.restapiUrl + '/lights/' + state.toString(), null).subscribe();
  }
}
