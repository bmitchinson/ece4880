import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { Radio, RadioGroup } from 'preact-fluid';

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
    cButton: boolean;
    fButton: boolean;
}

export default class Temp extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            displayTemp: null,
            sensorIsDisconnected: false,
            switchIsOn: true,
            cButton: true,
            fButton: false
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

    toggleFButtton = () => {
        if (!this.state.fButton || this.state.cButton) {
            this.setState(
                { fButton: !this.state.fButton },
                this.toggleCButtton
            );
        }
    };

    toggleCButtton = () => {
        if (!this.state.cButton || this.state.fButton) {
            this.setState(
                { cButton: !this.state.cButton },
                this.toggleFButtton
            );
        }
    };

    getTempText = () => {
        if (!this.state.displayTemp) {
            return '--°C';
        }
        if (this.state.sensorIsDisconnected || !this.state.switchIsOn) {
            return '--';
        }
        const celTemp = this.state.displayTemp;
        const fButton = this.state.fButton;
        const tempText = fButton ? (celTemp * 9) / 5 + 32 : celTemp;
        return fButton ? tempText.toFixed(1) + '°F' : tempText + '°C';
    };

    getTempHeaderText = () => {
        let tempHeader = 'Current Temp:';
        if (this.state.sensorIsDisconnected) {
            tempHeader = 'Unplugged Sensor';
        }
        if (!this.state.switchIsOn) {
            tempHeader = 'No Data Available (Switch Off)';
        }
        return tempHeader;
    };

    render() {
        return (
            <div>
                <header class={style.TempLabel}>
                    {this.getTempHeaderText()}
                </header>
                <p class={style.Temp}>{this.getTempText()}</p>
                <div class={style.Buttons}>
                    <Radio
                        checked={this.state.cButton}
                        value="Celsius"
                        label="Celsius"
                        bgColor="#90d7c2"
                        effect="circle"
                        onChange={this.toggleCButtton}
                    />
                    <div class={style.Space} />
                    <Radio
                        checked={this.state.fButton}
                        value="Fahrenheit"
                        label="Fahrenheit"
                        bgColor="#90d7c2"
                        effect="circle"
                        onChange={this.toggleFButtton}
                    />
                </div>
            </div>
        );
    }
}
