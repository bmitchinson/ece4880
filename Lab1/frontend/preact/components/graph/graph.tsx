import { h, Component } from 'preact';
import { database } from '../firebase/firebase';
import { TrendChart, TimestampArray } from 'preact-charts';

interface MyProps {}
interface MyState {
    dataArray: TimestampArray;
    dataCoverArray: TimestampArray;
}

export default class Graph extends Component<MyProps, MyState> {
    constructor() {
        super();
        const dataArray = getFakeDataOne();
        const dataCoverArray = getCoverArray(dataArray);

        this.state = { dataArray, dataCoverArray };
    }

    componentDidMount() {
        setInterval(this.addNewPoint, 1000);
    }

    addNewPoint = () => {
        const dataArrayEdit = this.state.dataArray;
        const time = new Date();
        dataArrayEdit.push({
            timestamp: time,
            temp: Math.random() * 60 - 10
        });
        dataArrayEdit.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
        dataArrayEdit.splice(0, 1);
        const dataCoverArrayEdit = getCoverArray(dataArrayEdit);

        this.setState({
            dataArray: dataArrayEdit,
            dataCoverArray: dataCoverArrayEdit
        });
    };

    render() {
        const { dataArray, dataCoverArray } = this.state;
        dataArray[0].temp = 0;
        dataArray[1].temp = 50;

        return (
            <TrendChart
                lineColour="#90d7c2"
                lineColourTwo="#ffffff"
                name="Temps"
                x="timestamp"
                y="temp"
                data={getBoundedDataArray(dataArray)}
                dataSetTwo={dataCoverArray}
                margin={{ top: 60, right: 80, left: 80, bottom: 60 }}
                axisControl={false}
                tooltip={false}
            />
        );
    }
}

const getBoundedDataArray = array => {
    return array.map(val => {
        if (val.temp > 50) {
            return {
                timestamp: new Date(val.timestamp),
                temp: 50
            };
        } else if (val.temp < 0) {
            return {
                timestamp: new Date(val.timestamp),
                temp: 0
            };
        } else return val;
    });
};

const getFakeDataOne = () => {
    const dataArray = [];
    for (let i = 0; i < 33; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: Math.random() * 60 - 10
        });
    }
    for (let i = 33; i < 66; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: -100
        });
    }
    for (let i = 66; i < 99; i += 1) {
        const time = new Date();
        time.setSeconds(time.getSeconds() - i);
        dataArray.push({
            timestamp: time,
            temp: Math.random() * 60 - 10
        });
    }
    dataArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    return dataArray;
};

const getCoverArray = dataArray => {
    const dataCoverArray = [];
    console.log('getCoverGot', dataArray);

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
    dataCoverArray.sort((a, b) => (a.timestamp < b.timestamp ? -1 : 1));
    return dataCoverArray;
};
