import { h, Component } from 'preact';
import { database } from '../firebase/firebase';

import style from './temp.scss';

const tempRef = database.collection('temps');
const tempObj = tempRef.orderBy('time', 'desc').limit(1);

const sensorRef = database.collection('toggles').doc('sensor');
const switchRef = database.collection('toggles').doc('switch');

interface MyProps {}
interface MyState {
    displayTemp: number;
    sensorIsDisconnected: boolean;
    switchIsOn: boolean;
}

export default class Temp extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            displayTemp: null,
            sensorIsDisconnected: false,
            switchIsOn: true
        };
    }

    componentDidMount() {
        tempObj.onSnapshot(snapShot => {
            snapShot.forEach(doc => {
                const temp = doc.data().temp.toFixed(1);
                this.setState({ displayTemp: temp });
            });
        });
        sensorRef.onSnapshot(doc => {
            const sensorIsDisconnected = doc.data().isDisconnected;
            this.setState(prev => ({ ...prev, sensorIsDisconnected }));
        });
        switchRef.onSnapshot(doc => {
            const switchIsOn = doc.data().isOn;
            this.setState(prev => ({ ...prev, switchIsOn }));
        });
    }

    render() {
        let tempHeader = 'Current Temp:';
        if (this.state.sensorIsDisconnected) {
            tempHeader = 'Unplugged Sensor';
        }
        if (!this.state.switchIsOn) {
            tempHeader = 'No Data Available (Switch Off)';
        }
        const currentTemp = this.state.displayTemp;
        const tempText =
            currentTemp && tempHeader === 'Current Temp:'
                ? currentTemp + '°C'
                : '--';
        console.log(this.state, tempHeader, tempText);
        return (
            <div>
                <header class={style.TempLabel}>{tempHeader}</header>
                <p class={style.Temp}>{tempText}</p>
            </div>
        );
    }
}
