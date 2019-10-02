import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';
import { CubeGrid } from 'styled-loaders';

import style from './graph.scss';

const openingFiveMinAgo = new Date(Date.now() - 300000);

const tempRef = database.collection('temps');
const tempObj = tempRef.where('time', '>=', openingFiveMinAgo).limit(300);
const newTempObj = tempRef.orderBy('time', 'desc').limit(1);

interface MyProps {}
interface MyState {
    loading: boolean;
    dataArray: TimestampArray;
    fiveMinExtent: Date[];
}

const LOWER_BOUND = 10;
const UPPER_BOUND = 50;

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const extent = this.refreshExtent();
        const dataArray = [];
        const time = new Date();
        dataArray.push({
            timestamp: time,
            temp: -100
        });

        this.state = { dataArray, loading: true, fiveMinExtent: extent };
    }

    componentDidMount() {
        const dataArray = this.state.dataArray;

        const fiveMinAgoSeconds = this.state.fiveMinExtent[0].getTime() / 1000;
        const zeroMinAgoSeconds = this.state.fiveMinExtent[1].getTime() / 1000;

        tempObj
            .get()
            .then(snap => {
                // Backfill the last 300 temps
                snap.forEach(doc => {
                    const seconds = doc.data().time.seconds;
                    const temp = doc.data().temp;
                    if (
                        seconds >= fiveMinAgoSeconds &&
                        seconds <= zeroMinAgoSeconds
                    ) {
                        dataArray.push({
                            timestamp: new Date(seconds * 1000),
                            temp: temp
                        });
                    }
                });
                for (let i = 0; i < dataArray.length - 1; i++) {
                    // if the point in front of it is more than 2 seconds away,
                    // add a -100 immedately after *the point* and immediately
                    // before the next
                    const nextPointIsNegative = dataArray[i + 1].temp === -100;
                    const currPointIsNegative = dataArray[i].temp === -100;
                    const nextPointUnixSeconds =
                        dataArray[i + 1].timestamp.getTime() / 1000;
                    const currPointUnixSeconds =
                        dataArray[i].timestamp.getTime() / 1000;
                    if (
                        nextPointUnixSeconds > currPointUnixSeconds + 3 &&
                        !currPointIsNegative &&
                        !nextPointIsNegative
                    ) {
                        const imedAfter = {
                            timestamp: new Date(
                                currPointUnixSeconds * 1000 + 200
                            ),
                            temp: -100
                        };
                        const imedBefore = {
                            timestamp: new Date(
                                nextPointUnixSeconds * 1000 - 200
                            ),
                            temp: -100
                        };
                        dataArray.splice(i + 1, 0, imedAfter);
                        dataArray.splice(i + 2, 0, imedBefore);
                    }
                }
                // Listen for new temps
                newTempObj.onSnapshot(snapShot => {
                    snapShot.forEach(doc => {
                        const dataArrayEdit = this.state.dataArray;
                        const newItemSec =
                            doc.data().time && doc.data().time.seconds;
                        if (newItemSec >= fiveMinAgoSeconds) {
                            dataArrayEdit.push({
                                temp: doc.data().temp,
                                timestamp: new Date(newItemSec * 1000)
                            });
                            this.setState({
                                dataArray: dataArrayEdit
                            });
                        }
                    });
                });
                setInterval(this.refreshExtent, 100);
                setInterval(this.addPointIfOld, 100);
                this.setState({ loading: false });
            })
            .catch(e => console.log('Error getting documents:', e));
    }

    refreshExtent = () => {
        const now = new Date();
        const fiveAgo = new Date();
        fiveAgo.setSeconds(fiveAgo.getSeconds() - 300);

        this.setState({ fiveMinExtent: [fiveAgo, now] });

        return [fiveAgo, now];
    };

    addPointIfOld = () => {
        const dataArrayEdit = this.state.dataArray;
        const fiveMinExtent = this.state.fiveMinExtent;
        const mostRecentPointUnixSeconds =
            dataArrayEdit[dataArrayEdit.length - 1].timestamp.getTime() / 1000;
        const currentTimeUnixSeconds = fiveMinExtent[1].getTime() / 1000;

        if (mostRecentPointUnixSeconds + 3 < currentTimeUnixSeconds) {
            dataArrayEdit.push({
                timestamp: new Date((mostRecentPointUnixSeconds + 3) * 1000),
                temp: -100
            });
            this.setState({ dataArray: dataArrayEdit });
        }
    };

    printDataArray = () => {
        console.log(this.state.dataArray);
    };

    render() {
        const { dataArray, loading, fiveMinExtent } = this.state;
        const dataCoverArray = getCoverArray(dataArray);

        return !loading ? (
            <div class={style.ContainGraph}>
                <div class={style.TopCRow}>
                    <div class={`${style.XAxisItem} ${style.C}`}>°C</div>
                </div>
                <div class={style.ContainChart}>
                    <TrendChart
                        extent={fiveMinExtent}
                        lineColour="#90d7c2"
                        lineColourTwo="#ffffff"
                        name="Temps"
                        x="timestamp"
                        y="temp"
                        data={getBoundedDataArray(dataArray)}
                        dataSetTwo={getBoundedDataArray(dataCoverArray)}
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

const getCoverArray = dataArray => {
    const dataCoverArray = [];

    dataCoverArray.push({
        timestamp: dataArray[0].timestamp,
        temp: 0
    });
    dataCoverArray.push({
        timestamp: dataArray[dataArray.length - 1].timestamp,
        temp: 0
    });

    dataArray.forEach((ele, index, dataArray) => {
        const canLookBehind: boolean = index !== 0;
        const canLookAhead: boolean = index !== dataArray.length - 1;
        const isNeg100: boolean = ele.temp === -100;

        if (isNeg100 && canLookBehind && dataArray[index - 1].temp !== -100) {
            const pastTemp = dataArray[index - 1].temp;
            const pastTimestamp = dataArray[index - 1].timestamp;
            const shiftTimestamp = new Date(pastTimestamp);
            shiftTimestamp.setMilliseconds(pastTimestamp.getMilliseconds() - 2);

            dataCoverArray.push({
                timestamp: shiftTimestamp,
                temp: 0
            });
            dataCoverArray.push({
                timestamp: pastTimestamp,
                temp: pastTemp
            });
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
        } else if (
            isNeg100 &&
            canLookAhead &&
            dataArray[index + 1].temp !== -100
        ) {
            const nextTemp = dataArray[index + 1].temp;
            const nextTimestamp = dataArray[index + 1].timestamp;
            const shiftTimestamp = new Date(nextTimestamp);
            shiftTimestamp.setMilliseconds(nextTimestamp.getMilliseconds() + 2);
            dataCoverArray.push({
                timestamp: ele.timestamp,
                temp: 0
            });
            dataCoverArray.push({
                timestamp: nextTimestamp,
                temp: nextTemp
            });
            dataCoverArray.push({
                timestamp: shiftTimestamp,
                temp: 0
            });
        }
    });
    return dataCoverArray;
};
