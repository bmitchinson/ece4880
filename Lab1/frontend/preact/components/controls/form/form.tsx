import { h, Component } from 'preact';
import { TextField, Button } from 'preact-fluid';

import style from './form.scss';

interface MyProps {
    userPermission?: boolean;
}
interface MyState {
    switch: boolean;
}

const theme = {
    primaryColor: '#90d7c2',
    linkColor: '#fff'
};

export default class Form extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            switch: false
        };
    }

    componentDidMount() {
        // TODO: Get switch value on construction and set
        // assign listener on switch in case of disable?
    }

    render() {
        // const userPermission = this.props.userPermission;
        // const activeStatus = userPermsion && this.state.switch;
        const gridPref = { columns: '1fr' };
        return (
            <div class={style.FormContainer}>
                <h2>Notification Controls:</h2>
                <div class={style.FieldsContainer}>
                    <TextField className={style.InputPad} hideLabel placeholder="Phone #" grid={gridPref} />
                    <TextField className={style.InputPad} hideLabel placeholder="Hight Temp Alert Message" grid={gridPref} />
                    <TextField className={style.InputPad} hideLabel placeholder="Low Temp Alert Message" grid={gridPref} />
                    <TextField className={style.InputPad} hideLabel placeholder="High Temp Threshold" grid={gridPref} />
                    <TextField className={style.InputPad} hideLabel placeholder="Low Temp Threshold" grid={gridPref} />
                    <Button className={style.InputPad} theme={theme} size={'large'} primary>
                        Submit
                    </Button>
                </div>
            </div>
        );
    }
}
