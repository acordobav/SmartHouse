import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class CameraService {

  constructor(private http: HttpClient) { }

  getPhoto() {
    return this.http.get<CameraResponse>(environment.restapiUrl + '/camera');
  }
}

export interface CameraResponse {
  location: string,
  last_photo: string
}