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
  private interval;

  constructor(private http: HttpClient) {
    //const subject = webSocket(environment.webSocketURL).subscribe();

    //this.connect({ reconnect: true });

    setInterval(() => this.fetchDoors(), 3000);
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

  /*
  private connect(cfg: { reconnect: boolean } = { reconnect: false }) {
    if (!this.socket || this.socket.closed) {
      this.socket = this.getNewWebSocket();
      const messages = this.socket.pipe(cfg.reconnect ? this.reconnect : o => o,
        tap({
          error: error => console.log(error),
        }), catchError(_ => EMPTY));
      messages.subscribe(
        (data) => {
          console.log(data);
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
          console.log('Connection closed');
          this.socket = undefined;
          this.connect({ reconnect: true });
        }
      },
    });
  }

  private close() {
    this.socket.complete();
  }
  */
}
