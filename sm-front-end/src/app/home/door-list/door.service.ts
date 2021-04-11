import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { EMPTY, Observable, Subject, timer } from 'rxjs';
import { Door } from 'src/app/models/door.model';
import { environment } from 'src/environments/environment';
import { webSocket, WebSocketSubject } from 'rxjs/webSocket';
import { catchError, delayWhen, retryWhen, tap } from 'rxjs/operators'

const RECONNECT_INTERVAL = 500000;

@Injectable({
  providedIn: 'root'
})
export class DoorService {
  private _doors = new Subject<Door[]>();
  private socket: WebSocketSubject<any>;

  constructor(private http: HttpClient) {
    this.connect({ reconnect: true });
  }

  get doors(): Observable<Door[]> {
    return this._doors.asObservable();
  }

  public fetchDoors() {
    this.http.get<Door[]>(environment.restapiUrl + '/door').subscribe(
      (doors) => {
        this._doors.next(doors);
      }
    );
  }


  private connect(cfg: { reconnect: boolean } = { reconnect: false }) {
    if (!this.socket || this.socket.closed) {
      this.socket = this.getNewWebSocket();
      const messages = this.socket.pipe(cfg.reconnect ? this.reconnect : o => o,
        tap({
          error: error => console.log(error),
        }), catchError(_ => EMPTY));
      messages.subscribe(
        () => {
          this.fetchDoors();
        }
      );
    }

  }

  private reconnect(observable: Observable<any>): Observable<any> {
    return observable.pipe(retryWhen(errors => errors.pipe(tap(val => console.log('Trying to reconnect to WebSocket', val)),
      delayWhen(_ => timer(RECONNECT_INTERVAL)))));
  }

  private getNewWebSocket() {
    return webSocket({
      url: environment.webSocketURL,
      closeObserver: {
        next: () => {
          this.socket = undefined;
          this.connect({ reconnect: true });
        }
      },
    });
  }

  public close() {
    this.socket.complete();
    this.socket = undefined;
  }
}
