import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';
import { CubeGrid } from 'styled-loaders';

import style from './graph.scss';

const tempRef = database.collection('temps');
const tempObj = tempRef.orderBy('time', 'desc').limit(300);
const newTempObj = tempRef.orderBy('time', 'desc').limit(1);

interface MyProps {}
interface MyState {
    loading: boolean;
    dataArray: TimestampArray;
}

const LOWER_BOUND = 10;
const UPPER_BOUND = 50;

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = [];
        for (let i = 300; i > 0; i -= 1) {
            const time = new Date();
            time.setSeconds(time.getSeconds() - i);
            dataArray.push({
                timestamp: time,
                temp: -100
            });
        }

        this.state = { dataArray, loading: true };
    }

    componentDidMount() {
        const dataArray = this.state.dataArray;

        const zeroMinAgo = Math.floor(
            dataArray[299].timestamp.getTime() / 1000
        );
        const fiveMinAgo = Math.floor(dataArray[0].timestamp.getTime() / 1200);

        tempObj
            .get()
            .then(snap => {
                // Backfill the last 300 temps
                snap.forEach(doc => {
                    const seconds = doc.data().time.seconds;
                    const temp = doc.data().temp;
                    if (seconds >= fiveMinAgo && seconds <= zeroMinAgo) {
                        const pos = 299 - (zeroMinAgo - seconds);
                        dataArray[pos] = {
                            timestamp: new Date(seconds * 1000),
                            temp: temp.toFixed(1)
                        };
                    }
                });
                // Listen for new temps
                newTempObj.onSnapshot(snapShot => {
                    snapShot.forEach(doc => {
                        const dataArrayEdit = this.state.dataArray;

                        const fiveMinAgo = Math.floor(
                            dataArrayEdit[0].timestamp.getTime() / 1000
                        );

                        if (doc.data().time) {
                            const newItemSec = doc.data().time.seconds;
                            if (newItemSec >= fiveMinAgo) {
                                dataArrayEdit.splice(0, 1);
                                dataArrayEdit.push({
                                    temp: doc.data().temp,
                                    timestamp: new Date(newItemSec * 1000)
                                });
                                this.setState({
                                    dataArray: dataArrayEdit
                                });
                            }
                        }
                    });
                });
                setInterval(this.addPointIfOld, 200);
                this.setState({ loading: false });
            })
            .catch(e => console.log('Error getting documents:', e));
    }

    addPointIfOld = () => {
        if (
            new Date().getTime() -
                this.state.dataArray[299].timestamp.getTime() >
            1000
        ) {
            const dataArrayEdit = this.state.dataArray;
            dataArrayEdit.splice(0, 1);
            dataArrayEdit.push({
                timestamp: new Date(),
                temp: -100
            });
            this.setState({
                dataArray: dataArrayEdit
            });
        }
    };

    render() {
        const { dataArray, loading } = this.state;

        return !loading ? (
            <div class={style.ContainGraph}>
                <div class={style.ContainChart}>
                    <TrendChart
                        lineColour="#90d7c2"
                        name="Temps"
                        x="timestamp"
                        y="temp"
                        data={getBoundedDataArray(dataArray)}
                        margin={{ top: 20, right: 20, left: 40, bottom: 10 }}
                        axisControl={false}
                        tooltip={false}
                    />
                </div>
                <div class={style.XAxisItems}>
                    <div class={`${style.XAxisItem} ${style.ThreeHundred}`}>
                        300
                    </div>
                    <div class={`${style.XAxisItem} ${style.TwoTwentyFive}`}>
                        225
                    </div>
                    <div class={`${style.XAxisItem} ${style.OneFifty}`}>
                        150
                    </div>
                    <div class={`${style.XAxisItem} ${style.SevenFive}`}>
                        75
                    </div>
                    <div class={`${style.XAxisItem} ${style.Zero}`}>0</div>
                </div>
                <div class={style.XAxisLabels}>
                    <div class={style.XAxisLabel}>(Seconds Ago)</div>
                </div>
            </div>
        ) : (
            <CubeGrid color="#90d7c2" size="60px" />
        );
    }
}

const getBoundedDataArray = array => {
    return array
        .map(val => {
            if (val.temp > UPPER_BOUND) {
                return {
                    timestamp: new Date(val.timestamp),
                    temp: UPPER_BOUND
                };
            } else if (val.temp < LOWER_BOUND) {
                return {
                    timestamp: new Date(val.timestamp),
                    temp: LOWER_BOUND
                };
            } else return val;
        })
        .sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
};
